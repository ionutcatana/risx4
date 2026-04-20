#include "acpi.h"
#include "alloc/conversions.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "limine.h"
#include "panic.h"
#include "risx.h"
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdpreq = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = LIMINE_API_REVISION
};

static struct xsdp *xsdp = NULL;

// compute a byte-sum checksum over len bytes. a valid acpi table sums to zero.
static uint8_t
checksum(const void *ptr, size_t len)
{
	const uint8_t *p = ptr;
	uint8_t sum = 0;

	for (size_t i = 0; i < len; i++)
		sum += p[i];
	return (sum);
}

bool
validsdtchecksum(const struct sdtheader *hdr)
{
	if (hdr == NULL)
		return (false);
	if (hdr->length < sizeof(struct sdtheader))
		return (false);
	return (checksum(hdr, hdr->length) == 0);
}

bool
validxsdp(struct xsdp *ptr)
{
	if (ptr == NULL)
		return (false);
	if (memcmp(ptr->signature, "RSD PTR ", 8) != 0)
		return (false);
	/* legacy checksum covers the first 20 bytes (required by spec). */
	if (checksum(ptr, 20) != 0)
		return (false);
	if (ptr->revision < ACPI_VERSION_SUBSEQUENT)
		return (false);
	/* extended checksum covers the full xsdp structure (acpi 2.0+). */
	if (checksum(ptr, ptr->length) != 0)
		return (false);
	if (ptr->xsdtaddr == 0)
		return (false);
	return (true);
}

void
parsexsdp(struct xsdp *ptr)
{
	struct sdtheader *xsdt;

	if (!validxsdp(ptr))
		panic("XSDP validation failed.");

	printf("ACPI: XSDP OEM=%.6s revision=%u\n", ptr->oemid,
	    ptr->revision);

	xsdt = virtual(ptr->xsdtaddr);
	parsexsdt(xsdt);
}

void
parsexsdt(struct sdtheader *hdr)
{
	uint64_t *entries;
	size_t nentries;

	if (hdr == NULL)
		panic("XSDT is NULL.");
	if (memcmp(hdr->signature, SDT_SIG_XSDT, 4) != 0)
		panic("XSDT signature mismatch.");
	if (!validsdtchecksum(hdr))
		panic("XSDT checksum invalid.");

	nentries = (hdr->length - sizeof(struct sdtheader)) /
	    sizeof(uint64_t);
	entries = (uint64_t *)((uint8_t *)hdr + sizeof(struct sdtheader));

	printf("ACPI: XSDT entries=%zu\n", nentries);

	for (size_t i = 0; i < nentries; i++) {
		struct sdtheader *table = virtual(entries[i]);

		if (table == NULL)
			continue;

		printf("ACPI: found table '%.4s' len=%u\n",
		    table->signature, table->length);

		if (memcmp(table->signature, SDT_SIG_FACP, 4) == 0)
			parsefadt((struct fadt *)table);
		else if (memcmp(table->signature, SDT_SIG_APIC, 4) == 0)
			parsemadt((struct madtheader *)table);
		else if (memcmp(table->signature, SDT_SIG_SRAT, 4) == 0)
			parsesrat((struct sratheader *)table);
	}
}

void
initacpi(void)
{
	if (rsdpreq.response == NULL)
		panic("ACPI: RSDP response is NULL.");

	xsdp = rsdpreq.response->address;
	parsexsdp(xsdp);
}

