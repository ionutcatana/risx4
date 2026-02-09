#ifndef X86_CONTEXT_H
#define X86_CONTEXT_H 1

#include <stdint.h>

// Saved registers for kernel context switches.
// Don't need to save all, just callee-saved.
typedef struct context {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
    uint64_t rip;
} context_t;

#endif
