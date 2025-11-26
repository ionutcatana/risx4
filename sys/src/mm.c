#include <core.h>
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

void initmm(void) {
    if (limine_memmap_request.response == NULL) {
        panic("null memmap response");
    }

    if (limine_hhdm_request.response == NULL) {
        panic("null hddm response");
    }

    // for (uint64_t i = 0; i < limine_memmap_request.response->entry_count; i++) {
    //     if (limine_memmap_request.response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
    //         memcpy(&memmap_entries[memmap_entry_count],
    //                limine_memmap_request.response->entries[i],
    //                sizeof(struct limine_memmap_entry));
    //         memmap_entry_count++;

    //         if (memmap_entry_count >= 128) {
    //             break;
    //         }
    //     }
    // }

    printf("HHDM offset: 0x%016lx\n", limine_hhdm_request.response->offset);
    printf("Usable memory map entries: %lu\n", limine_memmap_request.response->entry_count);
    for (size_t i = 0; i < limine_memmap_request.response->entry_count; i++) {
        switch (limine_memmap_request.response->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            printf("Area `%-25s: ", "usable`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_RESERVED:
            printf("Area `%-25s: ", "reserved`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            printf("Area `%-25s: ", "acpi reclaimable`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            printf("Area `%-25s: ", "acpi nvs`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            printf("Area `%-25s: ", "bad memory`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            printf("Area `%-25s: ", "bootloader reclaimable`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            printf("Area `%-25s: ", "kernel and modules`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            printf("Area `%-25s: ", "framebuffer`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_TABLES:
            printf("Area `%-25s: ", "acpi tables`");
            printf("Base: 0x%016lx, Length: 0x%016lx\n",
                limine_memmap_request.response->entries[i]->base,
                limine_memmap_request.response->entries[i]->length);
            break;
        }
    }
}

void initpmm(void) {
    initmm();
}
