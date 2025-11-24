#ifndef ATOMIC_H
#define ATOMIC_H 1

#include <stdint.h>

// arch/x86/atomic.S
uint64_t exchange(volatile uint64_t* addr, uint64_t newval);

#endif
