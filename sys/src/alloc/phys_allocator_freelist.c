#include "alloc/kpalloc.h"
#include "commonarch/paging.h"
#include "lib/string.h"
#include "panic.h"
#include <stddef.h>
#include <stdint.h>

static uint64_t* freelistaddr = NULL;

void initkpalloc(const struct limine_memmap_response* memmap) {
    enumeratememmap(memmap);

    uint64_t head_phys = 0;
    uint64_t tail_phys = 0;
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE)
            continue;

        uint64_t start = memmap->entries[i]->base;
        uint64_t end = memmap->entries[i]->base + memmap->entries[i]->length;
        for (uint64_t p = start; p < end; p += PAGE_SIZE) {
            // don't mark as `free` if the address is below 16mib
            if (p < 0x100000) continue;

                uint64_t *virt = (uint64_t*)virtual(p);
            *virt = 0; /* next = NULL for now */

            if (head_phys == 0) {
                head_phys = p;
                tail_phys = p;
            } else {
                    uint64_t *tvirt = (uint64_t*)virtual(tail_phys);
                *tvirt = p; /* link previous tail -> this frame */
                tail_phys = p;
            }
        }
    }

    if (head_phys == 0) freelistaddr = NULL;
        else freelistaddr = (uint64_t*)virtual(head_phys);
}

uint64_t allocframe(size_t count) {
    if (count == 0) panic("allocated 0 page frames.");

    uint64_t first_phys = 0;

    for (size_t i = 0; i < count; i++) {
        if (freelistaddr == NULL) panic("out of memory (freelist empty)");

            uint64_t this_phys = physical(freelistaddr);
        uint64_t next_phys = *(uint64_t*)freelistaddr;

        if (i == 0) first_phys = this_phys;

        /* advance freelist to next node */
        if (next_phys == 0)
            freelistaddr = NULL;
        else
                freelistaddr = (uint64_t*)virtual(next_phys);

        /* clear the allocated frame */
            memset(virtual(this_phys), 0, PAGE_SIZE);
    }

    return first_phys;
}

uint64_t allocmegaframe(size_t count);

void freeframe(uint64_t frameptr, size_t count) {
    if (count == 0) return;

    for (size_t i = 0; i < count; i++) {
        uint64_t p = frameptr + i * PAGE_SIZE;
           uint64_t head_phys = freelistaddr ? physical(freelistaddr) : 0;
           uint64_t *virt = (uint64_t*)virtual(p);
        *virt = head_phys;
           freelistaddr = (uint64_t*)virtual(p);
    }
}

void freemegaframe(uint64_t frameptr);


