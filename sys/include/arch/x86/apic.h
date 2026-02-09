#ifndef APIC_H
#define APIC_H 1

#include <stdint.h>

// Global variables populated by discovery
extern uintptr_t g_lapic_addr;
extern uintptr_t g_ioapic_addr;

// Function prototypes
void initapic_discovery(void);
void initlapic(void);
void acklapic(void);
void initioapic(void);

// LAPIC Registers (Offsets)
#define LAPIC_ID            0x0020
#define LAPIC_VER           0x0030
#define LAPIC_TPR           0x0080
#define LAPIC_EOI           0x00B0
#define LAPIC_SVR           0x00F0
#define LAPIC_ESR           0x0280
#define LAPIC_ICR_LOW       0x0300
#define LAPIC_ICR_HIGH      0x0310
#define LAPIC_LVT_TIMER     0x0320
#define LAPIC_LVT_LINT0     0x0350
#define LAPIC_LVT_LINT1     0x0360
#define LAPIC_LVT_ERROR     0x0370
#define LAPIC_TICR          0x0380
#define LAPIC_TCCR          0x0390
#define LAPIC_TDCR          0x03E0

// LAPIC Defines
#define LAPIC_SVR_ENABLE    0x100

// IOAPIC Registers
#define IOAPIC_ID           0x00
#define IOAPIC_VER          0x01
#define IOAPIC_ARB          0x02
#define IOAPIC_RED_TBL(n)   (0x10 + 2 * (n))

void ioapic_enable(uint8_t irq, uint8_t vector);

#endif
