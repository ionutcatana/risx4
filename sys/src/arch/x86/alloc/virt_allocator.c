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
    uintptr_t new_l4t = allocframe(1);  // level 4 table

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
            }

    uintptr_t stacks = allocmegaframe(1);
    mappage(new_l4t, STACK_BASE_VIRT, stacks, PAGE_PRESENT | PAGE_WRITABLE | PAGE_HUGE | PAGE_NO_EXECUTE);

//  loadcr3(new_l4t);   // the stack is fucked
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void mappage(uintptr_t l4taddr,
             uintptr_t va, uintptr_t pa, uint64_t flags) {
    return; // this is fucked
    uint64_t index;
    uint64_t* l4t = (uint64_t*)virtual(l4taddr);
    index = LVL4_INDEX(va);

    if (!(l4t[index] & PAGE_PRESENT)) {
        uintptr_t new_l3t = allocframe(1);
        l4t[index] = new_l3t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    uint64_t* l3t = (uint64_t*)virtual(l4t[index] & PTE_ADDRESS_MASK);
    index = LVL3_INDEX(va);

    if (!(l3t[index] & PAGE_PRESENT)) {
        uintptr_t new_l2t = allocframe(1);
        l3t[index] = new_l2t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    uint64_t* l2t = (uint64_t*)virtual(l3t[index] & PTE_ADDRESS_MASK);
    index = LVL2_INDEX(va);

    if (flags & PAGE_HUGE) {
        l2t[index] = pa | flags;
        return;
    }

    if (!(l2t[index] & PAGE_PRESENT)) {
        uintptr_t new_l1t = allocframe(1);
        l2t[index] = new_l1t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    uint64_t* l1t = (uint64_t*)virtual(l2t[index] & PTE_ADDRESS_MASK);
    index = LVL1_INDEX(va);

    l1t[index] = pa | flags;
}
