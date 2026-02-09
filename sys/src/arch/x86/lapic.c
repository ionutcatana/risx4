#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>
#include <arch/x86/apic.h>
#include <arch/x86/paging.h>
#include <risx.h>

#include <stdint.h>

static void lapic_write(uint32_t reg, uint32_t val) {
    volatile uint32_t* lapic = (volatile uint32_t*)virtual(g_lapic_addr);
    lapic[reg / 4] = val;
}

static uint32_t lapic_read(uint32_t reg) {
    volatile uint32_t* lapic = (volatile uint32_t*)virtual(g_lapic_addr);
    return lapic[reg / 4];
}

void acklapic(void) {
    lapic_write(LAPIC_EOI, 0);
}

void initlapic(void) {
    if (!g_lapic_addr) return;

    // Map LAPIC
    pagetable_t* pgtbl = (pagetable_t*)virtual(readkernelpgtbl());
    mappage(pgtbl, g_lapic_addr + hhdmoffset(), g_lapic_addr,
            PAGE_PRESENT | PAGE_WRITABLE | PAGE_CACHE_DISABLED | PAGE_NO_EXECUTE);

    // Enable LAPIC (Spurious Interrupt Vector = 0xFF | Enable Bit)
    lapic_write(LAPIC_SVR, 0xFF | LAPIC_SVR_ENABLE);

    uint32_t id = lapic_read(LAPIC_ID) >> 24;
    uint32_t ver = lapic_read(LAPIC_VER);
    printf("LAPIC initialized at 0x%016lx (ID: %d, Ver: %d)\n", g_lapic_addr, id, ver & 0xFF);

    // Initial Timer Setup (One-shot for now, or Periodic)
    // Map Timer to Vector 32 (0x20)
    lapic_write(LAPIC_LVT_TIMER, 0x20 | 0x20000); // 0x20000 = Periodic

    // Set Divisor to 16
    lapic_write(LAPIC_TDCR, 0x03);

    // Set Initial Count
    lapic_write(LAPIC_TICR, 10000000); // Arbitrary large value for now

    // Mask LINT0 and LINT1 (we use IOAPIC)
    lapic_write(LAPIC_LVT_LINT0, 0x10000);
    lapic_write(LAPIC_LVT_LINT1, 0x10000);

    // Clear Error Status
    lapic_write(LAPIC_ESR, 0);
    lapic_write(LAPIC_ESR, 0);

    // Ack any pending interrupts
    lapic_write(LAPIC_EOI, 0);

    // Set TPR to 0 to accept all interrupts
    lapic_write(LAPIC_TPR, 0);

    // printf("LAPIC initialized at 0x%016lx\n", g_lapic_addr); // Moved up
}
