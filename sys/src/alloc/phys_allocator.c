#include "alloc/bitmap.h"
#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "commonarch/paging.h"
#include "limine.h"
#include "risx.h"
#include "lib/string.h"
#include "lib/printf.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint32_t*    bitmap = NULL;
static uint32_t*    reclaimable_mask = NULL;
static uint64_t    mmend = 0;
static size_t       size = 0;
static size_t       freepages = 0;
static size_t       reclaimablepages = 0;
static uint64_t     totalpages = 0;
//static size_t     nextfree = 0;       // index of the next free page in the bitmap
//static uint64_t   nextfreepages = 0;  // number of free pages after index above
static uint64_t     offset_val = 0;

void initkpalloc(const uint64_t offset,
                 const struct limine_memmap_response* memmap) {
    enumeratememmap(memmap);
    offset_val = offset;

    for (size_t i = 0; i < memmap->entry_count; i++)
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE ||
            memmap->entries[i]->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
            uint64_t endaddr = memmap->entries[i]->base +
                                memmap->entries[i]->length;
            if (endaddr > mmend) mmend = endaddr;
        }

    totalpages = (mmend / PAGE_SIZE);
    size = (totalpages / 32); if (totalpages % 32 != 0) size++;

    for (size_t i = 0; i < memmap->entry_count; i++)
        // find a space for both bitmap and reclaimable mask
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE &&
            memmap->entries[i]->length >= size * 2 * sizeof(uint32_t)) {
            bitmap = (uint32_t*)(memmap->entries[i]->base + hhdmoffset());
            reclaimable_mask = (uint32_t*)(memmap->entries[i]->base + hhdmoffset()
                                                                    + size);
            memset(bitmap, 0xffffffff, size * sizeof(uint32_t));
            memset(reclaimable_mask, 0xffffffff, size * sizeof(uint32_t));
            break;
        }

    if (bitmap == NULL || reclaimable_mask == NULL)
        panic("bitmap (s) don't fit into memory.");

    const uint64_t bitmap_startaddr = (uint64_t)bitmap - hhdmoffset();
    const uint64_t bitmap_endaddr = bitmap_startaddr + size * sizeof(uint32_t);
    const uint64_t reclaimable_mask_startaddr = (uint64_t)reclaimable_mask - hhdmoffset();
    const uint64_t reclaimable_mask_endaddr = reclaimable_mask_startaddr + size * sizeof(uint32_t);

    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE)
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uint64_t currentaddr = memmap->entries[i]->base + j; // address of a page

                // don't mark as `free` if the address is below 16mib or it contains the bitmaps
                if ((currentaddr >= bitmap_startaddr && currentaddr < bitmap_endaddr) ||
                    (currentaddr >= reclaimable_mask_startaddr && currentaddr < reclaimable_mask_endaddr) ||
                    (currentaddr < 0x100000)) continue;

                unsetbit(bitmap, currentaddr / PAGE_SIZE);
                freepages++;
            }

        if (memmap->entries[i]->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) // acpi reclaimable can be added later
            for (uint64_t j = 0; j < memmap->entries[i]->length; j += PAGE_SIZE) {
                uint64_t currentaddr = memmap->entries[i]->base + j;
                setbit(reclaimable_mask, currentaddr / PAGE_SIZE);
                reclaimablepages++;
            }
    }

    printf("physical allocator summary:\n");
    printf("free pages: %u; bitmap size: %u\n", freepages, size);
    printf("bitmap location: %016lx; available ram: ~%uM\n", bitmap, freepages * PAGE_SIZE / 1024 / 1024);
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

uint64_t allocmegaframe(size_t count) {
    const size_t pages = count * PT_NELEMENTS;

    if (count == 0) panic("allocated 0 mega page frames.");
    if (pages > freepages) panic("requested more memory than available.");

    for (size_t i = 0; i + pages <= totalpages; i += pages) {
        bool found = true;
        for (size_t k = 0; k < pages; k++) if (checkbit(bitmap, i + k) != 0) {
            found = false;
            break;
        }

        if (found) {
            for (size_t k = 0; k < pages; k++) setbit(bitmap, i + k);
            freepages -= pages;
            uint64_t p = i * PAGE_SIZE;
            memset(virtual(p), 0, pages * PAGE_SIZE);
            return p;
        }
    }

    panic("out of contiguous memory.");
}

void freeframe(uint64_t frameptr, size_t count) {
    uint64_t frameidx = frameptr / PAGE_SIZE;
    for (uint64_t i = frameidx; i < frameidx + count; i++) unsetbit(bitmap, i);
    freepages += count;
}

void freemegaframe(uint64_t frameptr) {
    freeframe(frameptr, PT_NELEMENTS);
}

uint64_t hhdmoffset(void) {
    return offset_val;
}
