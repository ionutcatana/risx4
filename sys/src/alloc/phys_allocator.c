#include <alloc/bitmap.h>
#include <alloc/kpalloc.h>
#include <limine.h>
#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint32_t*   physbitmap = NULL;
size_t      size = 0;
size_t      freepages = 0;
size_t      totalpages = 0;

void initkpalloc(const uint64_t offset,
                 const struct limine_memmap_response* memmap) {
    uintptr_t maxtopaddr = 0;
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            uintptr_t topaddr = memmap->entries[i]->base +
                                memmap->entries[i]->length;
            if (topaddr > maxtopaddr) maxtopaddr = topaddr;
        }

    size = (maxtopaddr / PAGESIZE / sizeof(uint32_t)) + 1;
    totalpages = (maxtopaddr / PAGESIZE) + 1;
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmap->entries[i]->length >= size) {
            physbitmap = (uint32_t*)(memmap->entries[i]->base + offset);
            memset(physbitmap, 0xffffffff, size * sizeof(uint32_t));
            break;
        }

    if (physbitmap == NULL) panic("bitmap doesn't fit into memory.");

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (uint64_t p = 0; p < memmap->entries[i]->length; p += PAGESIZE) {
                uintptr_t physaddr = memmap->entries[i]->base + p;
                uintptr_t startaddr = (uintptr_t)physbitmap - offset;
                uintptr_t finaladdr = startaddr + size * sizeof(uint32_t);
                if (physaddr < startaddr || physaddr >= finaladdr) {
                    unsetbit(physbitmap, physaddr / PAGESIZE);
                    freepages++;
                }
            }
}

uintptr_t allocframe(size_t count) {
    if (count == 0) panic("allocated 0 page frames.");
    if (count > totalpages) panic("requested more memory than installed.");

    for (size_t i = 0; i + count <= totalpages; i++) {
        bool found = true;
        for (size_t k = 0; k < count; k++) if (checkbit(physbitmap, i + k) != 0) {
            found = false;
            i += k;
            break;
        }

        if (found) {
            for (size_t k = 0; k < count; k++) setbit(physbitmap, i + k);
            freepages -= count;
            return (i * PAGESIZE);
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(uintptr_t frameptr, size_t count) {
    uint64_t frameidx = frameptr / 4096;
    for (uint64_t i = frameidx; i < frameidx + count; i++) unsetbit(physbitmap, i);
    freepages += count;
}
