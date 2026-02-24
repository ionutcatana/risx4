#include "arch/x86_64/interrupts.h"
#include "arch/x86_64/specific/gdt.h" // for `RISX_CODE_SEG`
#include "arch/x86_64/specific/idt.h"
#include "arch/x86_64/specific/registers.h"
#include "lib/printf.h"
#include "panic.h"
#include "risx.h"
#include <stdint.h>

static idtr_t desc __attribute__((aligned(16)));
static volatile int64_t ninterrupts[NPROC] = {0}; // signed!
static volatile uint64_t intenabled[NPROC] = {0};

// from arch/x86_64/interrupts.S
extern uint64_t  _vectors[NENTRIES_IDT];
extern intdesc_t _idt[NENTRIES_IDT];

void intpush(void) {
    uint64_t rflags = readrflags();
    intdisable();

    uint64_t id = readlapicid();
    if (ninterrupts[id] == 0)
        intenabled[id] = rflags & RFLAGS_IF;
    ninterrupts[id]++;
}

void intpop(void) {
    uint64_t id = readlapicid();

    ninterrupts[id]--;
    if (ninterrupts[id] < 0)
        panic("decremented ninterrupts past 0.");

    if (readrflags() & RFLAGS_IF)
        panic("interrupts enabled flag is set.");

    if (ninterrupts[id] == 0 && intenabled[id])
        intenable();
}

void sethandler(size_t vector, uint64_t handler, uint8_t attributes) {
    _idt[vector].base_lower  = (uint16_t)(handler & 0xFFFF);
    _idt[vector].selector    = RISX_CODE_SEG;
    _idt[vector].ist         = 0;
    _idt[vector].attributes  = attributes;
    _idt[vector].base_middle = (uint16_t)((handler >> 16) & 0xFFFF);
    _idt[vector].base_upper  = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    _idt[vector].reserved    = 0;
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
        panic("general protection fault.");
    case X86_INTERRUPT_PF:
        printf("virtaddr: 0x%016lx\n", readcr2());
        panic("unresolved page fault.");
    case X86_INTERRUPT_MF:
        break;
    case X86_INTERRUPT_AC:
        break;
    case X86_INTERRUPT_MC:
        break;
    case X86_INTERRUPT_XF:
        break;
    default:
        panic("unimplemented interrupt handler.");
    }

    return;
}

void initidt(void) {
    desc.limit = sizeof(intdesc_t) * 256 - 1;
    desc.base = (uint64_t)_idt;

    for (size_t i = 0; i < NENTRIES_IDT; i++)
        sethandler(i,_vectors[i], IDT_INTERRUPT_GATE_RING0);

    loadidt(&desc);
}
