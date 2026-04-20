#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"
#include "commonarch/paging.h"
#include "lib/string.h"
#include "limine.h"
#include "memlayout.h"
#include "panic.h"
#include "risx.h"
#include "sync/spinlock.h"  // TODO: ADD SPINLOCKS HERE
#include <stddef.h>
#include <stdint.h>

#if defined(__x86_64__)
#include "arch/x86_64/paging.h"
#include "arch/x86_64/specific/registers.h"
#endif

extern char __kernel_start[];
extern char __kernel_end[];
extern char __kernel_size[];
extern char __text_start[]; // executable
extern char __text_end[];   // executable
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __data_end[];
extern char __bss_start[];
extern char __bss_end[];

extern uint64_t _guards[NCPU];
extern uint64_t _iguards[NCPU];

static struct pagetable* kerneltable_addr = NULL;

struct pagetable* kerneltable(void)
{
    return kerneltable_addr;
}

void initkvalloc(uint64_t base_physaddr, uint64_t base_virtaddr, struct limine_memmap_response* memmap)
{
    kerneltable_addr = virtual(allocframe(1));
    memset(kerneltable_addr, 0, sizeof(struct pagetable));

    /* map the entire physical memory into higher half using an offset        */
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
            memmap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY) {
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uint64_t curraddr = memmap->entries[i]->base + j;
                mappage(kerneltable_addr, base_virtaddr + curraddr, curraddr, PAGE_RISX_DATA);
            }
        }

    /* map the kernel's code and data                                         */
    for (uint64_t curr_virtaddr = base_virtaddr, curr_physaddr = base_physaddr; curr_virtaddr <= (uint64_t)__kernel_end; curr_physaddr += PAGE_SIZE, curr_virtaddr += PAGE_SIZE) {
        uint64_t currflags = 0;
        if (curr_virtaddr >= (uint64_t) __text_start && curr_virtaddr < (uint64_t) __text_end)
            currflags = PAGE_RISX_TEXT;
        else if (curr_virtaddr >= (uint64_t) __rodata_start && curr_virtaddr < (uint64_t) __rodata_end)
            currflags = PAGE_RISX_RODATA;
        else if ((curr_virtaddr >= (uint64_t) __data_start && curr_virtaddr < (uint64_t) __data_end) ||
                 (curr_virtaddr >= (uint64_t) __bss_start && curr_virtaddr < (uint64_t) __bss_end))
            currflags = PAGE_RISX_DATA;
        else
            break;

        mappage(kerneltable_addr, curr_virtaddr, curr_physaddr, currflags);
    }

    /* Unmap the guard pages for the 32 kernel and 32 interrupt stacks        */
    for (size_t i = 0; i < 32; i++) {
        unmappage(kerneltable_addr, _guards[i], 1);
        unmappage(kerneltable_addr, _iguards[i], 1);
    }
}

void unmappage(struct pagetable* globaltbl, uint64_t start_virtaddr, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        uint64_t virtaddr = start_virtaddr + i * PAGE_SIZE;

        uint64_t index = LVL4_INDEX(virtaddr);
        if (!(globaltbl->entries[index] & PAGE_PRESENT))
            continue;

        struct pagetable* lvl3tbl = virtual(globaltbl->entries[index] & PTE_ADDRESS_MASK);
        index = LVL3_INDEX(virtaddr);
        if (!(lvl3tbl->entries[index] & PAGE_PRESENT))
            continue;

        struct pagetable* lvl2tbl = virtual(lvl3tbl->entries[index] & PTE_ADDRESS_MASK);
        index = LVL2_INDEX(virtaddr);
        if (!(lvl2tbl->entries[index] & PAGE_PRESENT))
            continue;

        struct pagetable* lvl1tbl = virtual(lvl2tbl->entries[index] & PTE_ADDRESS_MASK);
        index = LVL1_INDEX(virtaddr);

        /* Clear the page table entry to unmap it                             */
        lvl1tbl->entries[index] = 0;
    }
}

void mappage(struct pagetable* globaltbl, uint64_t virtaddr, uint64_t physaddr, uint64_t flags)
{
    uint64_t index;
    index = LVL4_INDEX(virtaddr);
    if (!(globaltbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        globaltbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    struct pagetable* lvl3tbl = virtual(globaltbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL3_INDEX(virtaddr);
    if (!(lvl3tbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        lvl3tbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    struct pagetable* lvl2tbl = virtual(lvl3tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL2_INDEX(virtaddr);
    if (!(lvl2tbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        lvl2tbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    struct pagetable* lvl1tbl = virtual(lvl2tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL1_INDEX(virtaddr);

    lvl1tbl->entries[index] = physaddr | flags;
}

uint64_t* walk(struct pagetable* pagetable, uint64_t virtaddr)
{
    if (!pagetable) return NULL;

    uint64_t index = LVL4_INDEX(virtaddr);
    if (!(pagetable->entries[index] & PAGE_PRESENT)) {
        return NULL; /* Not mapped */
    }

    struct pagetable* lvl3tbl = virtual(pagetable->entries[index] & PTE_ADDRESS_MASK);
    index = LVL3_INDEX(virtaddr);
    if (!(lvl3tbl->entries[index] & PAGE_PRESENT)) {
        return NULL;
    }

    struct pagetable* lvl2tbl = virtual(lvl3tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL2_INDEX(virtaddr);
    if (!(lvl2tbl->entries[index] & PAGE_PRESENT)) {
        return NULL;
    }

    struct pagetable* lvl1tbl = virtual(lvl2tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL1_INDEX(virtaddr);

    /* return a pointer to the pte so the caller can read or modify it        */
    return &lvl1tbl->entries[index];
}

void mapmmio(uint64_t physaddr, size_t npages)
{
    uint64_t cr3 = readcr3();
    struct pagetable* active = virtual(cr3 & PTE_ADDRESS_MASK);

    uint64_t flags = PAGE_PRESENT
                   | PAGE_WRITABLE
                   | PAGE_CACHE_DISABLED
                   | PAGE_WRITE_THROUGH
                   | PAGE_NO_EXECUTE
                   | PAGE_GLOBAL;

    for (size_t i = 0; i < npages; i++) {
        uint64_t pa = physaddr + i * PAGE_SIZE;
        uint64_t va = (uint64_t)virtual(pa);
        mappage(active, va, pa, flags);
    }
}
