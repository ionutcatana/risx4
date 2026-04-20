#ifndef X86_64_SPECIFIC_IOAPIC_H
#define X86_64_SPECIFIC_IOAPIC_H 1

#include <stdint.h>

// ioapic register indices (accessed through the indirect register window).
#define IOAPIC_ID               0x00
#define IOAPIC_VER              0x01
#define IOAPIC_ARB              0x02
#define IOAPIC_REDTBL(n)        (0x10 + 2 * (n))

/* redirection entry bits                                                     */
#define IOAPIC_INT_MASKED       (1u << 16)
#define IOAPIC_INT_LEVEL        (1u << 15)
#define IOAPIC_INT_ACTIVELOW    (1u << 13)
#define IOAPIC_INT_LOGICAL      (1u << 11)

/* maximum ioapic redirection entries                                         */
#define IOAPIC_MAX_REDIR        24

// arch/x86_64/ioapic.c
void     initioapic(void);
void     ioapicroute(uint32_t irq, uint8_t vector, uint32_t lapic_id);
void     ioapicmask(uint32_t irq);
void     ioapicunmask(uint32_t irq);
uint32_t ioapicread(uint32_t reg);
void     ioapicwrite(uint32_t reg, uint32_t val);

#endif
