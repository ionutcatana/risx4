#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>
#include <arch/x86/apic.h>
#include <arch/x86/paging.h>
#include <risx.h>

#include <stdint.h>

static void ioapic_write(uint32_t reg, uint32_t val) {
    volatile uint32_t* ioapic = (volatile uint32_t*)virtual(g_ioapic_addr);
    ioapic[0] = reg;       // IOREGSEL
    ioapic[4] = val;       // IOWIN
}

static uint32_t ioapic_read(uint32_t reg) {
    volatile uint32_t* ioapic = (volatile uint32_t*)virtual(g_ioapic_addr);
    ioapic[0] = reg;
    return ioapic[4];
}

void ioapic_enable(uint8_t irq, uint8_t vector) {
    // Determine redirection table entry index
    uint32_t low_index  = IOAPIC_RED_TBL(irq);
    uint32_t high_index = IOAPIC_RED_TBL(irq) + 1;

    // Standard Redirection Entry:
    // Vector: 'vector'
    // Delivery Mode: 000 (Fixed)
    // Destination Mode: 0 (Physical)
    // Mask: 0 (Unmasked)

    uint32_t low_value  = vector;
    uint32_t high_value = (0 << 24); // Destination ID (usually 0 for BSP)

    ioapic_write(low_index, low_value);
    ioapic_write(high_index, high_value);
}

void initioapic(void) {
    if (!g_ioapic_addr) return;

    // Map IOAPIC
    pagetable_t* pgtbl = (pagetable_t*)virtual(readkernelpgtbl());
    mappage(pgtbl, g_ioapic_addr + hhdmoffset(), g_ioapic_addr,
            PAGE_PRESENT | PAGE_WRITABLE | PAGE_CACHE_DISABLED | PAGE_NO_EXECUTE);

    uint32_t ver = ioapic_read(IOAPIC_VER);
    uint32_t count = ((ver >> 16) & 0xFF) + 1;

    printf("IOAPIC Version 0x%02x, Entries: %d\n", ver & 0xFF, count);

    // Mask all entries initially
    for (uint32_t i = 0; i < count; i++) {
        ioapic_write(IOAPIC_RED_TBL(i), 0x10000); // Set Mask bit
    }

    // Example: Map Keyboard (IRQ 1) to Vector 33 (0x21)
    // Note: This relies on identity mapping (IRQ1 -> GSI 1) unless ISO overrides.
    // For now we assume standard ISA identity mapping.
    ioapic_enable(1, 0x21);

    printf("IOAPIC initialized at 0x%016lx\n", g_ioapic_addr);
}
