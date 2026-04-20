#include "acpi.h"
#include "lib/printf.h"
#include "lib/string.h"
#include <stddef.h>

bool
validdsdt(struct dsdt *ptr)
{
	if (ptr == NULL)
		return (false);
	if (memcmp(ptr->header.signature, SDT_SIG_DSDT, 4) != 0)
		return (false);
	if (!validsdtchecksum(&ptr->header))
		return (false);
	if (ptr->header.length < sizeof(struct sdtheader))
		return (false);
	return (true);
}

void
parsedsdt(struct dsdt *ptr)
{
	size_t amlsz;

	if (!validdsdt(ptr)) {
		printf("ACPI: DSDT validation failed, skipping.\n");
		return;
	}

	amlsz = ptr->header.length - sizeof(struct sdtheader);
	printf("ACPI: DSDT revision=%u OEM=%.6s AML_size=%zu\n", ptr->header.revision, ptr->header.oemid, amlsz);
}
