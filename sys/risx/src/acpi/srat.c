#include "acpi.h"
#include "lib/printf.h"
#include "lib/string.h"
#include <stddef.h>

bool
validsrat(struct sratheader *ptr)
{
	if (ptr == NULL)
		return (false);
	if (memcmp(ptr->header.signature, SDT_SIG_SRAT, 4) != 0)
		return (false);
	if (!validsdtchecksum(&ptr->header))
		return (false);
	if (ptr->header.length < sizeof(struct sratheader))
		return (false);
	return (true);
}

void
parsesrat(struct sratheader *ptr)
{
	uint8_t *base, *end;
	struct sratrecordheader *rec;

	if (!validsrat(ptr)) {
		printf("ACPI: SRAT validation failed, skipping.\n");
		return;
	}

	printf("ACPI: SRAT revision=%u\n", ptr->header.revision);

	base = (uint8_t *)ptr + sizeof(struct sratheader);
	end = (uint8_t *)ptr + ptr->header.length;

	while (base < end) {
		rec = (struct sratrecordheader *)base;

		if (rec->length < sizeof(struct sratrecordheader))
			break;
		if (base + rec->length > end)
			break;

		switch (rec->type) {
		case SRAT_RECORD_LAPIC_AFFINITY: {
			struct record_srat_lapic_affinity *r;
			uint32_t domain;

			r = (struct record_srat_lapic_affinity *)(base +
			    sizeof(struct sratrecordheader));
			domain = r->proximity_domain_lo |
			    ((uint32_t)r->proximity_domain_hi[0] << 8) |
			    ((uint32_t)r->proximity_domain_hi[1] << 16) |
			    ((uint32_t)r->proximity_domain_hi[2] << 24);
			printf("ACPI:  SRAT LAPIC apic=%u domain=%u flags=0x%08x\n",
                r->apic_id,
                domain,
                r->flags);

			break;
		}
		case SRAT_RECORD_MEMORY_AFFINITY: {
			struct record_srat_memory_affinity *r;
			uint64_t membase, memlen;

			r = (struct record_srat_memory_affinity *)(base +
			    sizeof(struct sratrecordheader));
			membase = ((uint64_t)r->base_address_hi << 32) |
			    r->base_address_lo;
			memlen = ((uint64_t)r->length_hi << 32) |
			    r->length_lo;
			printf("ACPI:  SRAT MEM domain=%u base=0x%016llx len=0x%016llx flags=0x%08x\n",
			    r->proximity_domain,
			    membase,
			    memlen,
			    r->flags);
			break;
		}
		case SRAT_RECORD_LX2APIC_AFFINITY: {
			struct record_srat_lx2apic_affinity *r;

			r = (struct record_srat_lx2apic_affinity *)(base +
			    sizeof(struct sratrecordheader));
			printf("ACPI:  SRAT x2APIC id=%u domain=%u flags=0x%08x\n",
			    r->lx2apic_id,
             r->proximity_domain,
			    r->flags);
			break;
		}
		default:
			printf("ACPI:  SRAT unknown record type=%u len=%u\n", rec->type, rec->length);
			break;
		}

		base += rec->length;
	}
}
