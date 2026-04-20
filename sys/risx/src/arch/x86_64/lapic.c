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
	lapicwrite(lapic_svr, lapic_svr_enable | vec_lapic_spurious);

	// mask lint0, lint1. the ioapic will handle external interrupts.
	// nmi is configured per the madt lapic nmi record.
	lapicwrite(lapic_lint0, lapic_lvt_masked);
	if (ai->nmi_lint == 1)
		lapicwrite(lapic_lint1, 0x400);         // nmi delivery mode
	else
		lapicwrite(lapic_lint0, 0x400);

	lapicwrite(lapic_esr, 0);   	            // clear error status
	lapicwrite(lapic_esr, 0);
	lapiceoi();                                 // ack any outstanding interrupts
	lapicwrite(lapic_tpr, 0);                   // set task priority to 0 so we accept all interrupts
	lapicwrite(lapic_error, vec_lapic_error);	// set error interrupt vector


	printf("[cpu %u] lapic initialized, id=%u.\n", lapicid(), lapicid());
}
