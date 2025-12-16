#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>
#include <arch/x86/paging.h>
#include <commonarch/paging.h>
#include <limine.h>
#include <risx.h>

#include <stddef.h>
#include <stdint.h>

static uint64_t offset_val = 0;

void initkvalloc(uint64_t physbase, uint64_t virtbase, uint64_t offset,
                 struct limine_memmap_response* memmap) {
    offset_val = offset;
    uintptr_t new_l4t = allocptframe();

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES)
            for (uint64_t p = 0; p < memmap->entries[i]->length; p += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + p;
                uintptr_t virtaddr = physaddr + offset_val;
                mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE);
            }

    // ... continue tomorrow
    (void)physbase;
    (void)virtbase;
}

uintptr_t allocptframe() {
    uintptr_t physaddr = allocframe(1);
    uintptr_t virtaddr = virtual(physaddr);
    memset((void*)virtaddr, 0, PAGE_SIZE);

    return physaddr;
}

void mappage(uintptr_t l4addr,
             uintptr_t va, uintptr_t pa, uint64_t flags) {
    uint64_t* table = (uint64_t*)physical(l4addr);

    // 4->3
    uint16_t l4i = LVL4_INDEX(va);
    if (!(table[l4i] & PAGE_PRESENT)) {
        uintptr_t new_l3t = allocptframe();
        table[l4i] = new_l3t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }
    table[l4i] = virtual(table[l4i] & PTE_PHYS_ADDR_MASK);

    // 3->2
    uint16_t l3i = LVL3_INDEX(va);
    if (!(table[l3i] & PAGE_PRESENT)) {
        uintptr_t new_l2t = allocptframe();
        table[l3i] = new_l2t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }
    table[l3i] = virtual(table[l3i] & PTE_PHYS_ADDR_MASK);

    // 2->1
    uint16_t l2i = LVL2_INDEX(va);
    if (flags & PAGE_HUGE) {
        table[l2i] = pa | flags;
        return;
    }

    if (!(table[l2i] & PAGE_PRESENT)) {
        uintptr_t new_l1i = allocptframe();
        table[l2i] = new_l1i | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    // 1
    uint16_t l1i = LVL1_INDEX(va);
    table[l1i] = pa | flags;
}

uint64_t offset() {
    return offset_val;
}
