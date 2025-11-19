#ifndef X86_ACPI_H
#define X86_ACPI_H 1

#include <stdint.h>

#define ACPI_VERSION_1 0            // 1.0
#define ACPI_VERSION_SUBSEQUENT 2   // 2.0-6.1

struct rsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
} __attribute__((packed));

struct xsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
    uint32_t length;
    uint64_t xsdtaddr;
    uint8_t xchecksum;
    uint8_t reserved[8];
} __attribute__((packed));

// arch/x86/acpi.c
void initacpi(void);
int acpiversion(void);

#endif
