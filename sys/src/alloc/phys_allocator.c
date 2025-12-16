#include <alloc/bitmap.h>
#include <alloc/kpalloc.h>
#include <commonarch/paging.h>
#include <limine.h>
#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint32_t*   bitmap = NULL;
uint32_t*   reclaimable_mask = NULL;
size_t      size = 0;
size_t      current = 0;
size_t      freepages = 0;
size_t      totalpages = 0;

void initkpalloc(const uint64_t offset,
                 const struct limine_memmap_response* memmap) {
    enumeratememmap(memmap);
    uintptr_t maxtopaddr = 0;
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            uintptr_t topaddr = memmap->entries[i]->base +
                                memmap->entries[i]->length;
            if (topaddr > maxtopaddr) maxtopaddr = topaddr;
        }

    size = (maxtopaddr / PAGE_SIZE / sizeof(uint32_t)) + 1;
    totalpages = (maxtopaddr / PAGE_SIZE) + 1;
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmap->entries[i]->length >= size) {
            bitmap = (uint32_t*)(memmap->entries[i]->base + offset);
            memset(bitmap, 0xffffffff, size * sizeof(uint32_t));
            break;
        }

    if (bitmap == NULL) panic("bitmap doesn't fit into memory.");

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (uint64_t p = 0; p < memmap->entries[i]->length; p += PAGE_SIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + p;
                uintptr_t startaddr = (uintptr_t)bitmap - offset;
                uintptr_t finaladdr = startaddr + size * sizeof(uint32_t);
                if (physaddr < startaddr || physaddr >= finaladdr) {
                    unsetbit(bitmap, physaddr / PAGE_SIZE);
                    freepages++;
                }
            }
}

uintptr_t allocframe(size_t count) {
    if (count == 0) panic("allocated 0 page frames.");
    if (count > totalpages) panic("requested more memory than installed.");

    for (size_t i = 0; i + count <= totalpages; i++) {
        bool found = true;
        for (size_t k = 0; k < count; k++) if (checkbit(bitmap, i + k) != 0) {
            found = false;
            i += k;
            break;
        }

        if (found) {
            for (size_t k = 0; k < count; k++) setbit(bitmap, i + k);
            freepages -= count;
            return (i * PAGE_SIZE);
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(uintptr_t frameptr, size_t count) {
    uint64_t frameidx = frameptr / 4096;
    for (uint64_t i = frameidx; i < frameidx + count; i++) unsetbit(bitmap, i);
    freepages += count;
}
