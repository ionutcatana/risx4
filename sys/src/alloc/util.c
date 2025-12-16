#include <alloc/kpalloc.h>
#include <risx.h>

void enumeratememmap(const struct limine_memmap_response* memmap) {
    for (size_t i = 0; i < memmap->entry_count; i++)
        switch (memmap->entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            printf("base 0x%016lx; length: 0x%016lx; type: USABLE\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_RESERVED:
            printf("base 0x%016lx; length: 0x%016lx; type: RESERVED\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            printf("base 0x%016lx; length: 0x%016lx; type: RECLAIMABLE (ACPI)\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            printf("base 0x%016lx; length: 0x%016lx; type: ACPI NVS\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            printf("base 0x%016lx; length: 0x%016lx; type: BAD\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            printf("base 0x%016lx; length: 0x%016lx; type: RECLAIMABLE (BOOTLOADER)\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
            printf("base 0x%016lx; length: 0x%016lx; type: EXECUTABLE & MODULES\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            printf("base 0x%016lx; length: 0x%016lx; type: FRAMEBUFFER\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        case LIMINE_MEMMAP_ACPI_TABLES:
            printf("base 0x%016lx; length: 0x%016lx; type: ACPI TABLES\n",
                memmap->entries[i]->base, memmap->entries[i]->length);
            break;
        default:
            printf("default");
            break;
        }
}
