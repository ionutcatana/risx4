#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/interrupts.h>
#include <arch/x86/registers.h>
#include <risx.h>

#include <stdint.h>
#include <stddef.h>

static idtsegdesc_t idt[IDT_SIZE];
idtr_t _idtr; // used in idt.S
extern uintptr_t vectors[]; // arch/x86/vectors.S

void loadidt(void);
void initidt(void) {
    _idtr.limit = (IDT_SIZE) * sizeof(idtsegdesc_t) - 1;
    _idtr.base = (uintptr_t)idt;

    for (size_t i = 0; i < IDT_SIZE; i++) {
        sethandler(i,vectors[i], IDT_INTERRUPT_GATE_RING0);
    }

    loadidt();
}

void sethandler(size_t vector, uintptr_t handler, uint8_t attributes) {
    idt[vector].base_lower = (uint16_t)(handler & 0xFFFF);
    idt[vector].selector = RISX_CODE_SEG;
    idt[vector].ist = 0;
    idt[vector].attributes = attributes;
    idt[vector].base_middle = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[vector].base_upper = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    idt[vector].reserved = 0;
}

void idispatch(trapframe_t* tf) {
    printf("Interrupt: %d; Error: %d\n", tf->vector, tf->error);

    // handle the interrupt after it has been announced on the serial port
    // exceptions 9 & 15 not defined yet
    switch (tf->vector) {
    case X86_INTERRUPT_DE:
        break;
    case X86_INTERRUPT_DB:
        break;
    case X86_INTERRUPT_NMI:
        break;
    case X86_INTERRUPT_BP:
        break;
    case X86_INTERRUPT_OF:
        break;
    case X86_INTERRUPT_BR:
        break;
    case X86_INTERRUPT_UD:
        break;
    case X86_INTERRUPT_NM:
        break;
    case X86_INTERRUPT_DF:
        break;
    case X86_INTERRUPT_TS:
        break;
    case X86_INTERRUPT_NP:
        break;
    case X86_INTERRUPT_SS:
        break;
    case X86_INTERRUPT_GP:
        break;
    case X86_INTERRUPT_PF:
        printf("virtaddr: 0x%016lx\n", readcr2());
        panic("unresolved page fault");
    case X86_INTERRUPT_MF:
        break;
    case X86_INTERRUPT_AC:
        break;
    case X86_INTERRUPT_MC:
        break;
    case X86_INTERRUPT_XF:
        break;
    default:
        panic("unimplemented interrupt handler");
    }

    return;
}

