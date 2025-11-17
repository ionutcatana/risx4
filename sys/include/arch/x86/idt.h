#ifndef X86_IDT_H
#define X86_IDT_H 1

#include <stddef.h>
#include <stdint.h>

#define IDT_SIZE 256
//                 bits:  76543210
#define INTERRUPT_GATE  0b00001110
#define TRAP_GATE       0b00001111
#define RING0           0b00000000
#define RING3           0b01100000
#define PRESENT         0b10000000

#define IDT_INTERRUPT_GATE_RING0 ((INTERRUPT_GATE) | (RING0) | (PRESENT))
#define IDT_INTERRUPT_GATE_RING3 ((INTERRUPT_GATE) | (RING3) | (PRESENT))
#define IDT_TRAP_GATE_RING0      ((TRAP_GATE)      | (RING0) | (PRESENT))
#define IDT_TRAP_GATE_RING3      ((TRAP_GATE)      | (RING3) | (PRESENT))

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

union idt_descriptor {
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
} __attribute__((packed));


// arch/x86/idt.S
void loadidt(void);
// arch/x86/idt.c
void initidt(void);
void sethandler(size_t vector, uintptr_t handler, uint8_t attributes);
void idispatch(void);
// arch/x86/istub.S
void istub(void);

#endif
