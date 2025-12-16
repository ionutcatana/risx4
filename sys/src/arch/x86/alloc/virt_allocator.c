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
void mappage(uintptr_t l4addr,
             uintptr_t va, uintptr_t pa, uint64_t flags) {
    return; // this was fucked
}
