#include <libk/kstdlib.h>
#include <limine.h>
#include <mm.h>

#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request limine_hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request limine_memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void initmm(void) {
    if (limine_hhdm_request.response == NULL ||
        limine_hhdm_request.response->offset != HHOFFSET) {
        abort();
    }

    if (limine_memmap_request.response == NULL) {
        abort();
    }
}
