#include "alloc/kpalloc.h"
#include "risx.h"
#include "lib/printf.h"

// TODO: remove later
void enumeratememmap(const struct limine_memmap_response* memmap)
{
    printf("memory map:\n");
    for (size_t i = 0; i < memmap->entry_count; i++)
        switch (memmap->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            printf("base 0x%016llx; length: 0x%016llx; type: USABLE\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_RESERVED:
            printf("base 0x%016llx; length: 0x%016llx; type: RESERVED\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            printf("base 0x%016llx; length: 0x%016llx; type: RECLAIMABLE (ACPI)\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            printf("base 0x%016llx; length: 0x%016llx; type: ACPI NVS\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            printf("base 0x%016llx; length: 0x%016llx; type: BAD\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            printf("base 0x%016llx; length: 0x%016llx; type: RECLAIMABLE (BOOTLOADER)\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            printf("base 0x%016llx; length: 0x%016llx; type: EXECUTABLE & MODULES\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            printf("base 0x%016llx; length: 0x%016llx; type: FRAMEBUFFER\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_TABLES:
            printf("base 0x%016llx; length: 0x%016llx; type: ACPI TABLES\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        default:
            printf("default");
            break;
        }
}
