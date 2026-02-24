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

static uint32_t*  bitmap     = NULL;
static physaddr_t mmend      = 0;
static size_t     size       = 0;
static uint64_t   freepages  = 0;
static uint64_t   totalpages = 0;

void initkpalloc(const struct limine_memmap_response* memmap) {
    enumeratememmap(memmap);

    // compute addr of the end of memory
    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            physaddr_t endaddr = memmap->entries[i]->base +
                                 memmap->entries[i]->length;
            if (endaddr > mmend) mmend = endaddr;
        }

    // determine bitmap size
    totalpages = ((mmend + 1) / PAGE_SIZE);
    size = (totalpages / sizeof(uint32_t)); if (totalpages % sizeof(uint32_t) != 0) size++;

    for (size_t i = 0; i < memmap->entry_count; i++)
        // find a space for bitmap
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmap->entries[i]->length >= size * sizeof(uint32_t)) {
            bitmap = virtual(memmap->entries[i]->base);
            memset(bitmap, 0xffffffff, size * sizeof(uint32_t));
            break;
        }

    if (bitmap == NULL) panic("bitmap doesn't fit in memory.");

    const physaddr_t bitmap_startaddr = physical(bitmap);
    const physaddr_t bitmap_endaddr = (bitmap_startaddr + size * sizeof(uint32_t) + 4095) & ~4095; // round up to nearest page

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (physaddr_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                physaddr_t currentaddr = memmap->entries[i]->base + j; // address of a page

                // don't mark as `free` if the address is below 16mib or it contains the bitmaps
                if ((currentaddr >= bitmap_startaddr && currentaddr <= bitmap_endaddr) ||
                    (currentaddr < 0x100000)) continue;

                unsetbit(bitmap, currentaddr / PAGE_SIZE);
                freepages++;
            }


    printf("bitmap location: %016lx\n", bitmap);
    printf("free pages: %lu (approx. %lu megabytes)\n", freepages, freepages * PAGE_SIZE / 1000000);
}

physaddr_t allocframe(size_t count) {
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
            physaddr_t p = i * PAGE_SIZE;
            memset(virtual(p), 0, count * PAGE_SIZE);
            return p;
        }
    }

    panic("out of contiguous memory.");
}

physaddr_t allocmegaframe(size_t count) {
    const size_t pages_per_mega = NENTRIES_PT;
    const size_t total_required_pages = count * pages_per_mega;

    if (count == 0) panic("allocated 0 mega page frames.");
    if (total_required_pages > freepages) panic("requested more memory than available.");

    // start i at a multiple of NENTRIES_PT to ensure alignment
    for (size_t i = 0; i + total_required_pages <= totalpages; i += pages_per_mega) {
        bool found = true;
        for (size_t k = 0; k < total_required_pages; k++) if (checkbit(bitmap, i + k) != 0) {
            found = false;
            break;
        }

        if (found) {
            for (size_t k = 0; k < total_required_pages; k++) setbit(bitmap, i + k);
            freepages -= total_required_pages;
            physaddr_t p = i * PAGE_SIZE;
            memset(virtual(p), 0, total_required_pages * PAGE_SIZE);
            return p;
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(physaddr_t frameptr, size_t count) {
    physaddr_t frameidx = frameptr / PAGE_SIZE;
    for (size_t i = frameidx; i < frameidx + count; i++) unsetbit(bitmap, i);
    freepages += count;
}

void freemegaframe(physaddr_t frameptr) {
    freeframe(frameptr, NENTRIES_PT);
}
