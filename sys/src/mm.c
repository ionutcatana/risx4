#include <limine.h>
#include <mm.h>
#include <risx.h>

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

void initmm(void) {
    if (limine_memmap_request.response == NULL) {
        panic("null memmap response");
    }

    if (limine_hhdm_request.response == NULL) {
        panic("null hddm response");
    }

    for (size_t i = 0; i < limine_memmap_request.response->entry_count; i++) {
        switch (limine_memmap_request.response->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            break;
        case LIMINE_MEMMAP_RESERVED:
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            break;
        case LIMINE_MEMMAP_ACPI_TABLES:
            break;
        }
    }
}

void initpmm(void) {
    initmm();
}
