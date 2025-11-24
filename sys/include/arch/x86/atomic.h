#ifndef X86_ATOMIC_H
#define X86_ATOMIC_H 1

#include <stdint.h>

// arch/x86/atomic.S
uint64_t exchange(volatile uint64_t* addr, uint64_t newval);

#endif
