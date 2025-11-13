#include <limine.h>
#include <mm.h>

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
    return;
}
