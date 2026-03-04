#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"
#include "commonarch/paging.h"
#include "lib/string.h"
#include "limine.h"
#include "memlayout.h"
#include "panic.h"
#include "risx.h"
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

static pagetable_t* kerneltable_addr = NULL;

pagetable_t* kerneltable() {
    return kerneltable_addr;
}

void initkvalloc(uint64_t base_physaddr,
                 uint64_t base_virtaddr, uint64_t stackbase,
                 struct limine_memmap_response* memmap) {
    (void)stackbase;
    kerneltable_addr = virtual(allocframe(1));
    memset(kerneltable_addr, 0, sizeof(pagetable_t));

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
    for (uint64_t curr_virtaddr = base_virtaddr, curr_physaddr = base_physaddr;
         curr_virtaddr <= (uint64_t)__kernel_end;
         curr_physaddr += PAGE_SIZE, curr_virtaddr += PAGE_SIZE) {
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

    /* map `NCPU` 16kib stacks and guard pages for kernel code                */
    /* map `NCPU` 16kib stacks and guard pages for interrupts                 */
    for (size_t cpu = 0; cpu < 2 * NCPU; cpu++) {
        uint64_t stack_virt = STACK0 - cpu * DISTANCE;
        uint64_t stack_physaddr = allocframe(4);
        for (size_t i = 0; i < 4; i++) {
            mappage(kerneltable_addr, stack_virt + i * PAGE_SIZE,
                                      stack_physaddr + i * PAGE_SIZE, PAGE_RISX_STACK);
        }
    }

    loadcr3(physical(kerneltable_addr));
}

void unmappage(pagetable_t* globaltbl, uint64_t start_virtaddr, size_t count) {
    (void) globaltbl;
    (void) start_virtaddr;
    (void) count;
}

void mappage(pagetable_t* globaltbl, uint64_t virtaddr,
                                     uint64_t physaddr, uint64_t flags) {
    uint64_t index;
    index = LVL4_INDEX(virtaddr);
    if (!(globaltbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        globaltbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    pagetable_t* lvl3tbl = virtual(globaltbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL3_INDEX(virtaddr);
    if (!(lvl3tbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        lvl3tbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    pagetable_t* lvl2tbl = virtual(lvl3tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL2_INDEX(virtaddr);
    if (!(lvl2tbl->entries[index] & PAGE_PRESENT)) {
        uint64_t p = allocframe(1);
        lvl2tbl->entries[index] = p | PAGE_TABLE_FLAGS;
    }

    pagetable_t* lvl1tbl = virtual(lvl2tbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL1_INDEX(virtaddr);

    lvl1tbl->entries[index] = physaddr | flags;
}
