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

// Helper to write to CR3
static void myloadcr3(uintptr_t pml4_phys) {
    __asm__ volatile ("mov %0, %%cr3" :: "r"(pml4_phys) : "memory");
}

void initkvalloc(uint64_t physbase, uint64_t virtbase,
                 struct limine_memmap_response* memmap) {
    // uintptr_t new_l4t = allocframe(1);  // level 4 table

    // for (size_t i = 0; i < memmap->entry_count; i++)
    //     if (memmap->entries[i]->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES) {
    //         for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
    //             uintptr_t physaddr = memmap->entries[i]->base + j;
    //             uintptr_t virtaddr = virtbase + (physaddr - physbase);

    //             // todo: elf64 parser
    //             mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE);
    //         }
    //     }

    // for (size_t i = 0; i < memmap->entry_count; i++)
    //     if (memmap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
    //         memmap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY)
    //         for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
    //             uintptr_t physaddr = memmap->entries[i]->base + j;
    //             uintptr_t virtaddr = physaddr + hhdmoffset();

    //             mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE | PAGE_NO_EXECUTE);
    //         }

    // uintptr_t stacks = allocmegaframe(1);
    // mappage(new_l4t, STACK_BASE_VIRT, stacks, PAGE_PRESENT | PAGE_WRITABLE | PAGE_HUGE | PAGE_NO_EXECUTE);

    // loadcr3(new_l4t);

    // Allocate the new PML4 (Level 4) table
    uintptr_t new_l4t = allocframe(1);

    // --- FIX 1: Map the Kernel Code & Data ---
    // We must use LIMINE_MEMMAP_KERNEL_AND_MODULES to find the kernel.
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES) {
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + j;
                // Calculate where the linker placed this code in virtual memory
                uintptr_t virtaddr = virtbase + (physaddr - physbase);

                // Map with EXECUTE permissions
                mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE);
            }
        }
    }

    // --- FIX 2: Map Physical Memory (HHDM) ---
    // Map all known memory regions to the higher half direct map (NX)
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
            memmap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY) {
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + j;
                uintptr_t virtaddr = physaddr + hhdmoffset();

                mappage(new_l4t, virtaddr, physaddr, PAGE_PRESENT | PAGE_WRITABLE | PAGE_NO_EXECUTE);
            }
        }
    }

    // --- FIX 3: Map Stacks ---
    uintptr_t stacks = allocmegaframe(1);
    mappage(new_l4t, STACK_BASE_VIRT, stacks, PAGE_PRESENT | PAGE_WRITABLE | PAGE_HUGE | PAGE_NO_EXECUTE);

    // Switch to the new page table
    myloadcr3(new_l4t);
}

void mappage(uintptr_t l4addr,
             uintptr_t va, uintptr_t pa, uint64_t flags) {
    // 1. Access the L4 table via Virtual Address (HHDM)
    uint64_t* l4 = (uint64_t*)virtual(l4addr);

    // 2. Walk L4 -> L3
    uint16_t l4i = LVL4_INDEX(va);
    if (!(l4[l4i] & PAGE_PRESENT)) {
        uintptr_t new_l3t = allocframe(1); // Returns physical, guaranteed zeroed
        l4[l4i] = new_l3t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    // Convert the L3 physical address in the entry to a virtual pointer
    uint64_t* l3 = (uint64_t*)virtual(l4[l4i] & PTE_PHYS_ADDR_MASK);

    // 3. Walk L3 -> L2
    uint16_t l3i = LVL3_INDEX(va);
    if (!(l3[l3i] & PAGE_PRESENT)) {
        uintptr_t new_l2t = allocframe(1);
        l3[l3i] = new_l2t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    uint64_t* l2 = (uint64_t*)virtual(l3[l3i] & PTE_PHYS_ADDR_MASK);

    // 4. Walk L2 -> L1 (or Map Huge Page)
    uint16_t l2i = LVL2_INDEX(va);

    // Handle 2MiB Huge Page mapping
    if (flags & PAGE_HUGE) {
        l2[l2i] = pa | flags;
        return;
    }

    // Standard 4KiB Page mapping
    if (!(l2[l2i] & PAGE_PRESENT)) {
        uintptr_t new_l1t = allocframe(1);
        l2[l2i] = new_l1t | PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER;
    }

    uint64_t* l1 = (uint64_t*)virtual(l2[l2i] & PTE_PHYS_ADDR_MASK);

    // 5. Map L1 Entry
    uint16_t l1i = LVL1_INDEX(va);
    l1[l1i] = pa | flags;

}
