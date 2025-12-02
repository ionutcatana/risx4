#include <limine.h>
#include <mm.h>
#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdmreq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 4
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmapreq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 4
};

uint8_t*    bitmap = NULL;
size_t      bitmap_size = 0;
size_t      freepages = 0;
uintptr_t   offset = 0;

void setbit(uint64_t index) {
    bitmap[index / 8] |= (1 << (index % 8));
}

void unsetbit(uint64_t index) {
    bitmap[index / 8] &= ~(1 << (index % 8));
}

int checkbit(uint64_t index) {
    return bitmap[index / 8] & (1 << (index % 8));
}

void initmm(void) {
    if (hhdmreq.response == NULL) {
        panic("null hddm response");
    }

    offset = (uintptr_t)hhdmreq.response->offset;

    if (memmapreq.response == NULL) {
        panic("null memmap response");
    }

    uintptr_t highest_addr = 0;
    size_t entry_count = memmapreq.response->entry_count;
    struct limine_memmap_entry** entries = memmapreq.response->entries;

    for (size_t i = 0; i < entry_count; i++) {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
            uintptr_t top = entries[i]->base + entries[i]->length;

            if (top > highest_addr) {
                highest_addr = top;
            }
        }
    }

    bitmap_size = (highest_addr / PAGESIZE / 8) + 1;
    freepages = (highest_addr / PAGESIZE) + 1;

    for (size_t i = 0; i < entry_count; i++) {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE &&
            entries[i]->length >= bitmap_size) {
            bitmap = (uint8_t*)(entries[i]->base + offset);
            memset(bitmap, 0xff, bitmap_size);
            break;
        }
    }

    if (bitmap == NULL) {
        panic("bitmap doesn't fit into memory");
    }

    for (size_t i = 0; i < entry_count; i++) {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE) {
            for (uint64_t j = 0; j < entries[i]->length; j += PAGESIZE) {
                uint64_t physaddr = entries[i]->base + j;
                uint64_t pos = physaddr / PAGESIZE;

                // the pages that contain the bitmap are never marked as free
                uintptr_t bitmap_phys_start = (uintptr_t)bitmap - offset;
                uintptr_t bitmap_phys_end = bitmap_phys_start + bitmap_size;
                if (physaddr >= bitmap_phys_start &&
                    physaddr < bitmap_phys_end) {
                    continue;
                }

                unsetbit(pos);
            }
        }
    }
}

uintptr_t allocframe(size_t count) {
    if (count < 0) {
        panic("allocated negative number of pages");
    }

    uintptr_t frameptr = 0;
    switch(count){
    case 0:
        panic("allocated 0 page frames");

    case 1: // finding a single free page frame is simple
        for (size_t i = 0; i < bitmap_size; i++) {
            if (bitmap[i] != 0xff) {
                for (size_t j = 0; j < 8; j++) {
                    if (checkbit(j) == 0) {
                        setbit(8 * i + j);
                        frameptr = (8 * i + j) * PAGESIZE;
                        return frameptr;
                    }
                }
            }
        }

    default:
        panic("not yet implemented");
    }

    return frameptr;
}

void freeframe(uintptr_t frameptr, size_t count) {
    uint64_t frame_index = frameptr / 4096;

    for (uint64_t i = frame_index; i < frame_index + count; i++) {
        unsetbit(frame_index);
    }
}
