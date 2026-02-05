#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>
#include <arch/x86/paging.h>
#include <arch/x86/registers.h>
#include <commonarch/paging.h>
#include <limine.h>
#include <risx.h>

#include <stddef.h>
#include <stdint.h>

void initkvalloc(uint64_t physbase, uint64_t virtbase,
                 struct limine_memmap_response* memmap) {
    pagetable_t* new_l4t = virtual(allocframe(1)); // level 4 table

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES) {
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + j;
                uintptr_t virtaddr = virtbase + (physaddr - physbase);

                // todo: elf64 parser
                mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE);
            }
        }

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
            memmap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY)
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + j;
                uintptr_t virtaddr = physaddr + hhdmoffset();

                mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE | PAGE_NO_EXECUTE);

                if (memmap->entries[i]->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
                    mappage(new_l4t, physaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE | PAGE_NO_EXECUTE);
                }
            }

    uintptr_t stacks = allocmegaframe(1);
    mappage(new_l4t, STACK_BASE_VIRT, stacks, PAGE_PRESENT | PAGE_WRITABLE | PAGE_HUGE | PAGE_NO_EXECUTE);

    loadcr3(physical(new_l4t));
}

void mappage(pagetable_t* globaltbl,
                    uintptr_t va, uintptr_t pa, uint64_t flags) {
    uint64_t index;

    index = LVL4_INDEX(va);
    if (!(globaltbl->entries[index] & PAGE_PRESENT)) {
        uintptr_t lowertbl = allocframe(1);
        globaltbl->entries[index] = lowertbl | PAGE_PRESENT | PAGE_WRITABLE;
    }

    pagetable_t* uppertbl = virtual(globaltbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL3_INDEX(va);
    if (!(uppertbl->entries[index] & PAGE_PRESENT)) {
        uintptr_t lowertbl = allocframe(1);
        uppertbl->entries[index] = lowertbl | PAGE_PRESENT | PAGE_WRITABLE;
    }

    pagetable_t* middletbl = virtual(uppertbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL2_INDEX(va);
    if (flags & PAGE_HUGE) {
        if (middletbl->entries[index] & PAGE_PRESENT)
            panic("mapping huge page over existing entry.");
        middletbl->entries[index] = pa | flags;
        return;
    }

    if ((middletbl->entries[index] & PAGE_PRESENT) &&
        (middletbl->entries[index] & PAGE_HUGE))
        panic("4kib page allocation collided with existing 2mib page.");

    if (!(middletbl->entries[index] & PAGE_PRESENT)) {
        uintptr_t lowertbl = allocframe(1);
        middletbl->entries[index] = lowertbl | PAGE_PRESENT | PAGE_WRITABLE;
    }

    pagetable_t* pagetbl = virtual(middletbl->entries[index] & PTE_ADDRESS_MASK);
    index = LVL1_INDEX(va);

    pagetbl->entries[index] = pa | flags;
}

