#include "alloc/bitmap.h"
#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "commonarch/paging.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "limine.h"
#include "panic.h"
#include "risx.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MEGABYTE 1000000
#define MEBIBYTE 0x100000

static uint32_t* bitmap     = NULL;
static uint64_t  mmendptr   = 0;
static size_t    size       = 0;
static uint64_t  freepages  = 0;
static uint64_t  totalpages = 0;

void initkpalloc(const struct limine_memmap_response* memmap) {
    enumeratememmap(memmap);

    // compute addr of the end of memory
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
            memmap->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY) {
            uint64_t endaddr = memmap->entries[i]->base +
                               memmap->entries[i]->length;
            if (endaddr > mmendptr) mmendptr = endaddr;
        }

    // determine bitmap size
    totalpages = ((mmendptr + 1) / PAGE_SIZE);
    size = (totalpages / sizeof(uint32_t)); if (totalpages % sizeof(uint32_t) != 0) size++;

    for (size_t i = 0; i < memmap->entry_count; i++)
        // find a space for bitmap
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmap->entries[i]->length >= size * sizeof(uint32_t)) {
            bitmap = virtual(memmap->entries[i]->base);
            memset(bitmap, UINT32_MAX, size * sizeof(uint32_t));
            break;
        }

    if (bitmap == NULL) panic("bitmap doesn't fit in memory.");

    const uint64_t bitmap_startaddr = physical(bitmap);
    const uint64_t bitmap_endaddr = (bitmap_startaddr + size * sizeof(uint32_t) + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1); // round up to nearest page

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uint64_t currentaddr = memmap->entries[i]->base + j; // address of a page

                // don't mark as `free` if the address is below 1mib or it contains the bitmaps
                if ((currentaddr >= bitmap_startaddr && currentaddr <= bitmap_endaddr) ||
                    (currentaddr < MEGABYTE)) continue;

                unsetbit(bitmap, currentaddr / PAGE_SIZE);
                freepages++;
            }


    printf("bitmap location: %016lx\n", bitmap);
    printf("free pages: %lu (approx. %lu megabytes)\n", freepages, freepages * PAGE_SIZE / MEGABYTE);
}

uint64_t allocframe(size_t count) {
    if (count == 0) panic("allocated 0 page frames.");
    if (count > freepages) panic("requested more memory than available.");

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
            uint64_t p = i * PAGE_SIZE;
            memset(virtual(p), 0, count * PAGE_SIZE);
            return p;
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(uint64_t frameptr, size_t count) {
    uint64_t frameidx = frameptr / PAGE_SIZE;
    for (size_t i = frameidx; i < frameidx + count; i++) unsetbit(bitmap, i);
    freepages += count;
}

