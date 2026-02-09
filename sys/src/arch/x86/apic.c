#include <arch/x86/acpi.h>
#include <arch/x86/apic.h>
#include <risx.h>

uintptr_t g_lapic_addr = 0;
uintptr_t g_ioapic_addr = 0;

void initapic_discovery(void) {
    madt_t* madt = (madt_t*)acpifindtable(ACPI_SIGNATURE_MADT);
    if (!madt) {
        panic("MADT table not found. Cannot determine APIC configuration.");
    }

    printf("MADT found at 0x%016lx\n", (uintptr_t)madt);
    g_lapic_addr = madt->lapicaddr;
    printf("Local APIC Address: 0x%08lx\n", g_lapic_addr);

    uint8_t* start = madt->entries;
    uint8_t* end   = (uint8_t*)madt + madt->header.length;

    while (start < end) {
        madt_header_t* entry = (madt_header_t*)start;
        switch (entry->type) {
            case 0: { // Local APIC
                madt_lapic_t* lapic = (madt_lapic_t*)entry;
//                printf("  -> LAPIC: Processor ID %d, APIC ID %d\n",
//                       lapic->processor_id, lapic->apic_id);
                (void)lapic;
                break;
            }
            case 1: { // IO APIC
                madt_ioapic_t* ioapic = (madt_ioapic_t*)entry;
                g_ioapic_addr = ioapic->ioapic_addr;
                printf("  -> IOAPIC: ID %d, Address 0x%08x, GSI Base %d\n",
                       ioapic->apic_id, ioapic->ioapic_addr, ioapic->gsibase);
                break;
            }
            case 2: { // ISO
                madt_iso_t* iso = (madt_iso_t*)entry;
                printf("  -> ISO: Bus %d, Source %d, GSI %d\n",
                       iso->bus, iso->source, iso->gsi);
                // TODO: Store ISOs for redirection
                break;
            }
        }
        start += entry->length;
    }
}
