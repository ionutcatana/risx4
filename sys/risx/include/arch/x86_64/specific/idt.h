#ifndef X86_64_SPECIFIC_IDT_H
#define X86_64_SPECIFIC_IDT_H 1

#include <stddef.h>
#include <stdint.h>

//                         bits:  76543210
#define INTERRUPT_GATE  0x0E /* 0b00001110 */
#define TRAP_GATE       0x0F /* 0b00001111 */
#define RING0           0x00 /* 0b00000000 */
#define RING3           0x60 /* 0b01100000 */
#define PRESENT         0x80 /* 0b10000000 */

#define IDT_INTERRUPT_GATE_RING0 ((INTERRUPT_GATE) | (RING0) | (PRESENT))
#define IDT_INTERRUPT_GATE_RING3 ((INTERRUPT_GATE) | (RING3) | (PRESENT))
#define IDT_TRAP_GATE_RING0      ((TRAP_GATE)      | (RING0) | (PRESENT))
#define IDT_TRAP_GATE_RING3      ((TRAP_GATE)      | (RING3) | (PRESENT))

#define NENTRIES_IDT    (256)

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

typedef union {
    struct {
        uint16_t base_lower;
        uint16_t selector;
        uint8_t ist;
        uint8_t attributes;
        uint16_t base_middle;
        uint32_t base_upper;
        uint32_t reserved;
    };
    struct {
        uint64_t lo;
        uint64_t hi;
    };
} __attribute__((packed)) intdesc_t;

// arch/x86_86/interrupts.S
void loadidt(struct idtr* desc);

// arch/x86_86/interrupts.c
void initidt(void);

#endif
