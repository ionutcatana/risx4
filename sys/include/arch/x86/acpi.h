#ifndef X86_ACPI_H
#define X86_ACPI_H 1

#include <stdint.h>

#define ACPI_VERSION_1 0            // 1.0
#define ACPI_VERSION_SUBSEQUENT 2   // 2.0-6.1

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
} __attribute__((packed)) rsdp_t;

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdpaddr;
    uint32_t length;
    uint64_t xsdtaddr;
    uint8_t xchecksum;
    uint8_t reserved[8];
} __attribute__((packed)) xsdp_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oemtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
} sdpheader_t;

// New: MADT Definitions
#define ACPI_SIGNATURE_MADT "APIC"

typedef struct {
    sdpheader_t header;
    uint32_t lapicaddr;
    uint32_t flags;
    uint8_t entries[];
} __attribute__((packed)) madt_t;

typedef struct {
    uint8_t type;
    uint8_t length;
} __attribute__((packed)) madt_header_t;

typedef struct {
    madt_header_t header;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__((packed)) madt_lapic_t;

typedef struct {
    madt_header_t header;
    uint8_t apic_id;
    uint8_t reserved;
    uint32_t ioapic_addr;
    uint32_t gsibase;
} __attribute__((packed)) madt_ioapic_t;

typedef struct {
    madt_header_t header;
    uint8_t bus;
    uint8_t source;
    uint32_t gsi;
    uint16_t flags;
} __attribute__((packed)) madt_iso_t;

typedef struct {
    sdpheader_t header;
    uint32_t sdtaddr[];
} rsdt;

typedef struct  {
    sdpheader_t header;
    uint64_t sdtaddr[];
} xsdt;

// arch/x86/acpi.c
void    initacpi(void);
int     acpiversion(void);
void*   acpifindtable(const char* signature);

// arch/x86/apic.c
void    initapic_discovery(void);

#endif
