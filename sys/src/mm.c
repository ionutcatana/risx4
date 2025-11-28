#include <limine.h>
#include <mm.h>
#include <risx.h>

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
uintptr_t   offset = 0;

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

    for (size_t i = 0; i < entry_count; i++) {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE &&
            entries[i]->length >= bitmap_size) {
            bitmap = (uint8_t*)(entries[i]->base + offset);
            memset(bitmap, 0xff, bitmap_size);
            break;
        }
    }
}

void initpmm(void) {
    initmm();
}
