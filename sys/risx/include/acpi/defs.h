#ifndef PRIVATE_ACPI_DEFS_H
#define PRIVATE_ACPI_DEFS_H 1

#include <stdint.h>

struct xsdp;
struct xsdt;
struct fadt;
struct madt;
struct dsdt;

// acpi/core.c
void initacpi(void);
void parsexsdp(struct xsdp* ptr);
void parsexsdt(struct xsdt* ptr);

bool validxsdp(struct xsdp* ptr);
bool validxsdt(struct xsdt* ptr);

// acpi/fadt.c
void parsefadt(struct fadt* ptr);
bool validfadt(struct fadt* ptr);

// acpi/madt.c
void parsemadt(struct madt* ptr);
bool validmadt(struct madt* ptr);

// acpi/dsdt.c
void parsedsdt(struct dsdt* ptr);
bool validdsdt(struct dsdt* ptr);

#endif
