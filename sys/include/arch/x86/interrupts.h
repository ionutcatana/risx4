#ifndef X86_INTERRUPTS_H
#define X86_INTERRUPTS_H 1

#include <commonarch/interrupts.h>

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

#endif
