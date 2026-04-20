#ifndef ACPI_H
#define ACPI_H 1

#include "acpi/constants.h"
#include "acpi/defs.h"
#include "acpi/types.h"

// struct rsdt {
//     struct sdtheader header;
//     uint32_t sdtaddr[(header.length - sizeof(struct sdtheader)) / 4];
// } __attribute__((packed));
// struct xsdt {
//     struct sdtheader header;
//     uint64_t sdtaddr[(header.length - sizeof(struct sdtheader)) / 8];
// } __attribute__((packed));
// struct reserved_madt_record {
// } __attribute__((packed));
// #define MADT_RECORD_FLAG_POLARITY_NO_OVERRIDE (0)
// #define MADT_RECORD_FLAG_POLARITY_DEFAULT (0)
// #define MADT_RECORD_FLAG_POLARITY_ACTIVE_HIGH_OVERRIDE (1)
// #define MADT_RECORD_FLAG_POLARITY_ACTIVE_LOW_OVERRIDE (3)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_NO_OVERRIDE (0)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_DEFAULT (0)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_EDGE_TRIGGERED (1)
// #define MADT_RECORD_FLAG_TRIGGER_MODE_LEVEL_TRIGGERED (3)

#endif
