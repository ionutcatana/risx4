#include "arch/x86_64/specific/lapic.h"
#include "arch/x86_64/interrupts.h"
#include "acpi/apicinfo.h"
#include "alloc/conversions.h"
#include "lib/printf.h"
#include "panic.h"
#include <stdint.h>

static volatile uint32_t *lapic_base;

uint32_t
lapicread(uint32_t off)
{
	return lapic_base[off / 4];
}

void
lapicwrite(uint32_t off, uint32_t val)
{
	lapic_base[off / 4] = val;
	(void)lapic_base[LAPIC_ID / 4]; // fence the write
}

uint32_t
lapicid(void)
{
	return lapicread(LAPIC_ID) >> 24;
}

void
lapiceoi(void)
{
	lapicwrite(LAPIC_EOI, 0);
}

void
initlapic(void)
{
	struct apicinfo *ai;

	ai = getapicinfo();
	if (ai->lapic_addr == 0)
		panic("lapic base address is zero.");

	lapic_base = (volatile uint32_t *)virtual(ai->lapic_addr);

	// enable apic, set spurious interrupt vector
	lapicwrite(LAPIC_SVR, LAPIC_SVR_ENABLE | VEC_LAPIC_SPURIOUS);

	// mask lint0, lint1. the ioapic will handle external interrupts.
	// nmi is configured per the madt lapic nmi record.
	lapicwrite(LAPIC_LINT0, LAPIC_LVT_MASKED);
	if (ai->nmi_lint == 1)
		lapicwrite(LAPIC_LINT1, 0x400);         // nmi delivery mode
	else
		lapicwrite(LAPIC_LINT0, 0x400);

	lapicwrite(LAPIC_ESR, 0);                   // clear error status
	lapicwrite(LAPIC_ESR, 0);
	lapiceoi();                                 // ack any outstanding interrupts
	lapicwrite(LAPIC_TPR, 0);                   // set task priority to 0 so we accept all interrupts
	lapicwrite(LAPIC_ERROR, VEC_LAPIC_ERROR);   // set error interrupt vector

//  printf("[CPU %u] lapic initialized, id=%u.\n", lapicid(), lapicid());
}
