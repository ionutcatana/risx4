#ifndef X86_64_SPECIFIC_LAPIC_H
#define X86_64_SPECIFIC_LAPIC_H 1

#include <stdint.h>

// local apic register offsets. all registers are accessed at 16-byte aligned
// offsets from the lapic mmio base.
#define LAPIC_ID                0x020
#define LAPIC_VER               0x030
#define LAPIC_TPR               0x080
#define LAPIC_EOI               0x0B0
#define LAPIC_SVR               0x0F0
#define LAPIC_ESR               0x280
#define LAPIC_ICRLO             0x300
#define LAPIC_ICRHI             0x310
#define LAPIC_TIMER             0x320
#define LAPIC_THERMAL           0x330
#define LAPIC_PERF              0x340
#define LAPIC_LINT0             0x350
#define LAPIC_LINT1             0x360
#define LAPIC_ERROR             0x370
#define LAPIC_TICR              0x380   // timer initial count register
#define LAPIC_TCCR              0x390   // timer current count register
#define LAPIC_TDCR              0x3E0   // timer divide configuration

/* spurious vector register bits                                              */
#define LAPIC_SVR_ENABLE        (1u << 8)

/* LVT entry masks */
#define LAPIC_LVT_MASKED        (1u << 16)

/* timer modes (bits 18:17 of LVT timer register)                             */
#define LAPIC_TIMER_ONESHOT     0x00000000
#define LAPIC_TIMER_PERIODIC    0x00020000

/* timer divide values for TDCR                                               */
#define LAPIC_TDCR_DIV1         0x0B
#define LAPIC_TDCR_DIV2         0x00
#define LAPIC_TDCR_DIV4         0x01
#define LAPIC_TDCR_DIV8         0x02
#define LAPIC_TDCR_DIV16        0x03
#define LAPIC_TDCR_DIV32        0x08
#define LAPIC_TDCR_DIV64        0x09
#define LAPIC_TDCR_DIV128       0x0A

/* delivery mode for ICR                                                      */
#define LAPIC_ICR_INIT          0x00000500
#define LAPIC_ICR_STARTUP       0x00000600
#define LAPIC_ICR_LEVEL         0x00008000
#define LAPIC_ICR_ASSERT        0x00004000
#define LAPIC_ICR_DEASSERT      0x00000000
#define LAPIC_ICR_DELIVS        0x00001000

// arch/x86_64/lapic.c
void     initlapic(void);
void     lapiceoi(void);
uint32_t lapicid(void);
uint32_t lapicread(uint32_t off);
void     lapicwrite(uint32_t off, uint32_t val);

#endif
