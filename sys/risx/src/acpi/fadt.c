#include "acpi.h"
#include "alloc/conversions.h"
#include "lib/printf.h"
#include "lib/string.h"
#include <stddef.h>

bool
validfadt(struct fadt *ptr)
{

	if (ptr == NULL)
		return (false);
	if (memcmp(ptr->header.signature, SDT_SIG_FACP, 4) != 0)
		return (false);
	if (!validsdtchecksum(&ptr->header))
		return (false);
	if (ptr->header.length < sizeof(struct fadt))
		return (false);
	return (true);
}

void
parsefadt(struct fadt *ptr)
{
	struct dsdt *dsdt;

	if (!validfadt(ptr)) {
		printf("ACPI: FADT validation failed, skipping.\n");
		return;
	}

    printf("ACPI: FADT revision=%u profile=%u sci_int=%u\n",
	    ptr->header.revision,
	    ptr->preferred_power_management_profile,
	    ptr->sci_interrupt);

	/* Use the 64-bit X_DSDT pointer (ACPI 2.0+ only). */
	if (ptr->x_dsdt != 0) {
		dsdt = virtual(ptr->x_dsdt);
		parsedsdt(dsdt);
	} else {
		printf("ACPI: FADT has no DSDT pointer.\n");
	}
}
