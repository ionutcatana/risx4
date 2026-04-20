#include "acpi.h"
#include "lib/printf.h"
#include "lib/string.h"
#include <stddef.h>

bool
validmadt(struct madtheader *ptr)
{
	if (ptr == NULL)
		return (false);
	if (memcmp(ptr->header.signature, SDT_SIG_APIC, 4) != 0)
		return (false);
	if (!validsdtchecksum(&ptr->header))
		return (false);
	if (ptr->header.length < sizeof(struct madtheader))
		return (false);
	return (true);
}

void
parsemadt(struct madtheader *ptr)
{
	uint8_t *base, *end;
	struct madtrecordheader *rec;

	if (!validmadt(ptr)) {
		printf("ACPI: MADT validation failed, skipping.\n");
		return;
	}

	printf("ACPI: MADT LAPIC_addr=0x%08x flags=0x%08x\n",
	    ptr->local_apic_addr, ptr->flags);

	base = (uint8_t *)ptr + sizeof(struct madtheader);
	end = (uint8_t *)ptr + ptr->header.length;

	while (base < end) {
		rec = (struct madtrecordheader *)base;

		if (rec->record_length < sizeof(struct madtrecordheader))
			break;
		if (base + rec->record_length > end)
			break;

		switch (rec->entry_type) {
		case MADT_RECORD_LAPIC: {
			struct record_lapic *r;

			r = (struct record_lapic *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC cpu=%u id=%u flags=0x%08x\n",
			    r->acpi_processor_id, r->acpi_id, r->flags);
			break;
		}
		case MADT_RECORD_IO_APIC: {
			struct record_io_apic *r;

			r = (struct record_io_apic *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  IOAPIC id=%u addr=0x%08x gsi=%u\n",
			    r->io_apic_id, r->io_apic_address,
			    r->global_system_interrupt_base);
			break;
		}
		case MADT_RECORD_IO_APIC_INTERRUPT_SOURCE_OVERRIDE: {
			struct record_io_apic_iso *r;

			r = (struct record_io_apic_iso *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  ISO bus=%u irq=%u gsi=%08x flags=0x%04x\n",
			    r->bus_source, r->irq_source,
			    r->global_system_interrupt, r->flags);
			break;
		}
		case MADT_RECORD_IO_APIC_NMI_SOURCE: {
			struct record_io_apic_nmis *r;

			r = (struct record_io_apic_nmis *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  NMI_SRC src=%u gsi=%08x flags=0x%04x\n",
			    r->nmi_source, r->global_system_interrupt,
			    r->flags);
			break;
		}
		case MADT_RECORD_LAPIC_NMI: {
			struct record_io_lapic_nmi *r;

			r = (struct record_io_lapic_nmi *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC_NMI cpu=%u lint=%u flags=0x%04x\n",
			    r->acpi_processord_id, r->lint_number,
			    r->flags);
			break;
		}
		case MADT_RECORD_LAPIC_ADDRESS_OVERRIDE: {
			struct record_io_lapic_ao *r;

			r = (struct record_io_lapic_ao *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC_ADDR_OVR addr=0x%016llx\n",
			    (unsigned long long)r->lapic_address);
			break;
		}
		case MADT_RECORD_LX2APIC: {
			struct record_io_lx2apic *r;

			r = (struct record_io_lx2apic *)(base +
			    sizeof(struct madtrecordheader));
			printf("ACPI:  x2APIC id=%u flags=0x%x\n",
			    r->lx2apic_id, r->flags);
			break;
		}
		default:
			printf("ACPI:  MADT unknown record type=%u len=%u\n",
			    rec->entry_type, rec->record_length);
			break;
		}

		base += rec->record_length;
	}
}
