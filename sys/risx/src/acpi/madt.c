#include "acpi.h"
#include "acpi/apicinfo.h"
#include "lib/printf.h"
#include "lib/string.h"
#include <stddef.h>

static struct apicinfo apic_info;

struct apicinfo *
getapicinfo(void)
{
	return &apic_info;
}

bool
validmadt(struct madtheader *ptr)
{
	if (ptr == NULL)
		return false;
	if (memcmp(ptr->header.signature, SDT_SIG_APIC, 4) != 0)
		return false;
	if (!validsdtchecksum(&ptr->header))
		return false;
	if (ptr->header.length < sizeof(struct madtheader))
		return false;

	return true;
}

// walk madt records and populate the global apicinfo structure.
void
collectmadt(struct madtheader *ptr)
{
	uint8_t *base, *end;
	struct madtrecordheader *rec;

	if (!validmadt(ptr)) {
		printf("ACPI: MADT validation failed, skipping.\n");
		return;
	}

	apic_info.lapic_addr = (uint64_t)ptr->local_apic_addr;
	apic_info.ncpus = 0;
	apic_info.nioapics = 0;
	apic_info.nisos = 0;
	apic_info.nmi_lint = 1; /* default: LINT1 */
	apic_info.nmi_flags = 0;

	printf("ACPI: MADT LAPIC_addr=0x%08x flags=0x%08x\n", ptr->local_apic_addr, ptr->flags);

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

			r = (struct record_lapic *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC cpu=%u id=%u flags=0x%08x\n",
			    r->acpi_processor_id,
                r->acpi_id,
                r->flags);

			if ((r->flags & (LAPIC_FLAG_ENABLED | LAPIC_FLAG_ONLINE)) &&
                apic_info.ncpus < NCPU) {
				struct lapicinfo *li;

				li = &apic_info.cpus[apic_info.ncpus];
				li->acpi_processor_id = r->acpi_processor_id;
				li->apic_id = r->acpi_id;
				li->flags = r->flags;
				apic_info.ncpus++;
			}
			break;
		}
		case MADT_RECORD_IO_APIC: {
			struct record_io_apic *r;

			r = (struct record_io_apic *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  IOAPIC id=%u addr=0x%08x gsi=%u\n",
			    r->io_apic_id, r->io_apic_address,
			    r->global_system_interrupt_base);

			/* use the first IOAPIC only */
			if (apic_info.nioapics == 0) {
				apic_info.ioapic.id = r->io_apic_id;
				apic_info.ioapic.address = r->io_apic_address;
				apic_info.ioapic.gsi_base = r->global_system_interrupt_base;
			}
			apic_info.nioapics++;
			break;
		}
		case MADT_RECORD_IO_APIC_INTERRUPT_SOURCE_OVERRIDE: {
			struct record_io_apic_iso *r;

			r = (struct record_io_apic_iso *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  ISO bus=%u irq=%u gsi=%08x flags=0x%04x\n",
			    r->bus_source, r->irq_source,
			    r->global_system_interrupt, r->flags);

			if (apic_info.nisos < MAX_IOAPIC_ISO) {
				struct isoinfo *iso;

				iso = &apic_info.isos[apic_info.nisos];
				iso->bus = r->bus_source;
				iso->irq = r->irq_source;
				iso->gsi = r->global_system_interrupt;
				iso->flags = r->flags;
				apic_info.nisos++;
			}
			break;
		}
		case MADT_RECORD_IO_APIC_NMI_SOURCE: {
			struct record_io_apic_nmis *r;

			r = (struct record_io_apic_nmis *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  NMI_SRC src=%u gsi=%08x flags=0x%04x\n",
			    r->nmi_source,
                r->global_system_interrupt,
			    r->flags);
			break;
		}
		case MADT_RECORD_LAPIC_NMI: {
			struct record_io_lapic_nmi *r;

			r = (struct record_io_lapic_nmi *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC_NMI cpu=%u lint=%u flags=0x%04x\n",
			    r->acpi_processord_id,
                r->lint_number,
			    r->flags);

			apic_info.nmi_lint = r->lint_number;
			apic_info.nmi_flags = r->flags;
			break;
		}
		case MADT_RECORD_LAPIC_ADDRESS_OVERRIDE: {
			struct record_io_lapic_ao *r;

			r = (struct record_io_lapic_ao *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  LAPIC_ADDR_OVR addr=0x%016llx\n", r->lapic_address);

			apic_info.lapic_addr = r->lapic_address;
			break;
		}
		case MADT_RECORD_LX2APIC: {
			struct record_io_lx2apic *r;

			r = (struct record_io_lx2apic *)(base + sizeof(struct madtrecordheader));
			printf("ACPI:  x2APIC id=%u flags=0x%x\n", r->lx2apic_id, r->flags);
			break;
		}
		default:
			printf("ACPI:  MADT unknown record type=%u len=%u\n", rec->entry_type, rec->record_length);
			break;
		}

		base += rec->record_length;
	}
}

void
parsemadt(struct madtheader *ptr)
{
	collectmadt(ptr);
}
