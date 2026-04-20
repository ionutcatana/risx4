#ifndef PRIVATE_ACPI_DEFS_H
#define PRIVATE_ACPI_DEFS_H 1

#include <stdbool.h>
#include <stdint.h>

struct xsdp;
struct xsdt;
struct sdtheader;
struct fadt;
struct madt;
struct madtheader;
struct dsdt;
struct sratheader;

// acpi/core.c
void initacpi(void);
void parsexsdp(struct xsdp *ptr);
void parsexsdt(struct sdtheader *hdr);
bool validsdtchecksum(const struct sdtheader *hdr);
bool validxsdp(struct xsdp *ptr);

// acpi/fadt.c
void parsefadt(struct fadt *ptr);
bool validfadt(struct fadt *ptr);

// acpi/madt.c
void parsemadt(struct madtheader *ptr);
bool validmadt(struct madtheader *ptr);

// acpi/dsdt.c
void parsedsdt(struct dsdt *ptr);
bool validdsdt(struct dsdt *ptr);

// acpi/srat.c
void parsesrat(struct sratheader *ptr);
bool validsrat(struct sratheader *ptr);

#endif
