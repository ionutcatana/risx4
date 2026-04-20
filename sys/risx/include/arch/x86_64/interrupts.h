#ifndef X86_64_EXTENDED_INTERRUPTS_H
#define X86_64_EXTENDED_INTERRUPTS_H 1

#include "commonarch/interrupts.h"
#include <stdint.h>

//                                                                     errcode?
#define X86_INTERRUPT_DE    (0)     // division error               // no
#define X86_INTERRUPT_DB    (1)     // debug                        // no
#define X86_INTERRUPT_NMI   (2)     // non maskable interrupt       // no
#define X86_INTERRUPT_BP    (3)     // breakpoint                   // no
#define X86_INTERRUPT_OF    (4)     // overflow                     // no
#define X86_INTERRUPT_BR    (5)     // bound range exceeded         // no
#define X86_INTERRUPT_UD    (6)     // invalid opcode               // no
#define X86_INTERRUPT_NM    (7)     // device not available         // no
#define X86_INTERRUPT_DF    (8)     // double fault                 // yes = 0
#define X86_INTERRUPT_TS    (10)    // invalid tss                  // yes
#define X86_INTERRUPT_NP    (11)    // segment not present          // yes
#define X86_INTERRUPT_SS    (12)    // stack segment fault          // yes
#define X86_INTERRUPT_GP    (13)    // general protection           // yes
#define X86_INTERRUPT_PF    (14)    // page fault                   // yes
#define X86_INTERRUPT_MF    (16)    // floating point unit error    // no
#define X86_INTERRUPT_AC    (17)    // alignment check              // yes
#define X86_INTERRUPT_MC    (18)    // machine check                // no
#define X86_INTERRUPT_XF    (19)    // simd error                   // no

// idt vector assignments for the x86_64 apic interrupt model.
// vectors 32-47 are used for ioapic-routed irqs (isa mapping).
#define IRQ_BASE            32
#define IRQ_TIMER_PIT       (IRQ_BASE + 0)  // ISA IRQ 0 -> vector 32
#define IRQ_KEYBOARD        (IRQ_BASE + 1)  // ISA IRQ 1 -> vector 33
#define IRQ_COM2            (IRQ_BASE + 3)  // ISA IRQ 3 -> vector 35
#define IRQ_COM1            (IRQ_BASE + 4)  // ISA IRQ 4 -> vector 36

// vector 48 is the lapic timer.
#define VEC_LAPIC_TIMER     48
#define VEC_LAPIC_ERROR     49

// vector 255 is the lapic spurious interrupt.
#define VEC_LAPIC_SPURIOUS  255

// overrides commonarch/interrupts.h
struct trapframe {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t vector;
    uint64_t error;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

#endif
