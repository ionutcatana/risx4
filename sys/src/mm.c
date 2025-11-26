#include <libk/kstdlib.h>
#include <libk/string.h>
#include <mm.h>
#include <risx.h>

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request limine_hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 4
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request limine_memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 4
};

struct limine_memmap_entry memmap_entries[128];
uint64_t memmap_entry_count = 0;
uint64_t hhdm_offset = 0;

void initmm(void) {
    if (limine_memmap_request.response == NULL) {
        panic("null memmap response");
    }

    if (limine_hhdm_request.response == NULL) {
        panic("null hddm response");
    }

    for (uint64_t i = 0; i < limine_memmap_request.response->entry_count; i++) {
        if (limine_memmap_request.response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            memcpy(&memmap_entries[memmap_entry_count],
                   limine_memmap_request.response->entries[i],
                   sizeof(struct limine_memmap_entry));
            memmap_entry_count++;

            if (memmap_entry_count >= 128) {
                break;
            }
        }
    }

    hhdm_offset = limine_hhdm_request.response->offset;
}

void initpmm(void) {
    initmm();
}
