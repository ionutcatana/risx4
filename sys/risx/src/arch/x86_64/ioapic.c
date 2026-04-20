#include "arch/x86_64/specific/ioapic.h"
#include "arch/x86_64/interrupts.h"
#include "acpi/apicinfo.h"
#include "acpi/constants.h"
#include "alloc/conversions.h"
#include "lib/printf.h"
#include "panic.h"
#include <stdint.h>

static volatile uint32_t *ioapic_base;

uint32_t
ioapicread(uint32_t reg)
{
    ioapic_base[0] = reg;
    return ioapic_base[4];
}

void
ioapicwrite(uint32_t reg, uint32_t val)
{
    ioapic_base[0] = reg;
    ioapic_base[4] = val;
}

// look up the gsi (global system interrupt) for a given isa irq, taking
// interrupt source overrides into account.
static uint32_t
isairq_to_gsi(struct apicinfo *ai, uint8_t irq, uint16_t *flags)
{
    for (uint32_t i = 0; i < ai->nisos; i++) {
        if (ai->isos[i].irq == irq) {
            *flags = ai->isos[i].flags;
            return (ai->isos[i].gsi);
        }
    }

    *flags = 0;
    return ((uint32_t)irq);
}

// route an isa irq to a given idt vector on a given lapic, respecting
// any interrupt source overrides from the madt.
void
ioapicroute(uint32_t irq, uint8_t vector, uint32_t lapic_id)
{
    struct apicinfo *ai;
    uint16_t flags;
    uint32_t gsi;
    uint64_t entry;
    uint32_t lo, hi;

    ai = getapicinfo();
    gsi = isairq_to_gsi(ai, (uint8_t)irq, &flags);

    entry = (uint64_t)vector;

    /* polarity */
    if ((flags & MADT_FLAG_POLARITY_MASK) == MADT_FLAG_POLARITY_ACTIVE_LOW)
        entry |= IOAPIC_INT_ACTIVELOW;

    /* trigger mode */
    if ((flags & MADT_FLAG_TRIGGER_MASK) == MADT_FLAG_TRIGGER_LEVEL)
        entry |= IOAPIC_INT_LEVEL;

    /* destination LAPIC */
    entry |= ((uint64_t)lapic_id << 56);

    lo = (uint32_t)(entry & 0xFFFFFFFF);
    hi = (uint32_t)(entry >> 32);

    ioapicwrite(IOAPIC_REDTBL(gsi) + 1, hi);
    ioapicwrite(IOAPIC_REDTBL(gsi), lo);
}

void
ioapicmask(uint32_t irq)
{
    uint32_t val;

    val = ioapicread(IOAPIC_REDTBL(irq));
    ioapicwrite(IOAPIC_REDTBL(irq), val | IOAPIC_INT_MASKED);
}

void
ioapicunmask(uint32_t irq)
{
    uint32_t val;

    val = ioapicread(IOAPIC_REDTBL(irq));
    ioapicwrite(IOAPIC_REDTBL(irq), val & ~IOAPIC_INT_MASKED);
}

// initialize the ioapic.  mask all entries, then route specific irqs.
// must be called after collectmadt().
void
initioapic(void)
{
    struct apicinfo *ai;
    uint32_t maxredir, ver;

    ai = getapicinfo();
    if (ai->nioapics == 0)
        panic("no IOAPIC found in MADT.");

    ioapic_base = (volatile uint32_t *)virtual(
        (uint64_t)ai->ioapic.address);

    ver = ioapicread(IOAPIC_VER);
    maxredir = ((ver >> 16) & 0xFF) + 1;

    printf("IOAPIC: id=%u, version=0x%02x, max_redir=%u\n",
        ai->ioapic.id, ver & 0xFF, maxredir);

    /* mask all redirection entries */
    for (uint32_t i = 0; i < maxredir; i++) {
        ioapicwrite(IOAPIC_REDTBL(i) + 1, 0);
        ioapicwrite(IOAPIC_REDTBL(i), IOAPIC_INT_MASKED);
    }

    printf("IOAPIC: initialized, all entries masked.\n");
}
