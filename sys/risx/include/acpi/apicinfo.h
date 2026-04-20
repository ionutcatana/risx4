#ifndef ACPI_APICINFO_H
#define ACPI_APICINFO_H 1

#include "risx/config.h"
#include <stdbool.h>
#include <stdint.h>

struct madtheader;

// maximum number of IOAPIC interrupt source overrides we track.
#define MAX_IOAPIC_ISO 24

/* per-CPU LAPIC information harvested from the MADT                          */
struct lapicinfo {
    uint8_t  acpi_processor_id;
    uint8_t  apic_id;
    uint32_t flags;
};

/* per-IOAPIC information harvested from the MADT                             */
struct ioapicinfo {
    uint8_t  id;
    uint32_t address;
    uint32_t gsi_base;
};

/* interrupt source override */
struct isoinfo {
    uint8_t  bus;
    uint8_t  irq;
    uint32_t gsi;
    uint16_t flags;
};

// collected apic topology from the madt.  all fields are filled by
// collectmadt() and consumed by initlapic(), initioapic(), inittimer().
struct apicinfo {
    /* LAPIC base address (may be overridden by address override record)      */
    uint64_t          lapic_addr;
    /* per-CPU LAPIC entries */
    uint32_t          ncpus;
    struct lapicinfo  cpus[NCPU];
    /* IOAPIC(s) -- we only use the first one */
    uint32_t          nioapics;
    struct ioapicinfo ioapic;
    /* interrupt source overrides */
    uint32_t          nisos;
    struct isoinfo    isos[MAX_IOAPIC_ISO];
    /* LAPIC NMI LINT# for each CPU (0xff = all CPUs) */
    uint8_t           nmi_lint;     /* LINT pin (0 or 1) */
    uint16_t          nmi_flags;
};

// acpi/madt.c
void collectmadt(struct madtheader *ptr);
struct apicinfo *getapicinfo(void);

#endif
