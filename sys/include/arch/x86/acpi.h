#ifndef X86_ACPI_H
#define X86_ACPI_H 1

#include <stdint.h>

#define ACPI_VERSION_1 0            // 1.0
#define ACPI_VERSION_SUBSEQUENT 2   // 2.0-6.1

struct rsdp_t {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
} __attribute__((packed));

struct xsdp_t {
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

struct sdpheader_t {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
};

struct rsdt {
    struct sdpheader_t header;
    uint32_t sdtaddr[];
};

struct xsdt {
    struct sdpheader_t header;
    uint64_t sdtaddr[];
};

// arch/x86/acpi.c
void initacpi(void);
int acpiversion(void);

#endif
