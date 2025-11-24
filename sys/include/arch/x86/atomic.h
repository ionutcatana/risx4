#ifndef X86_ATOMIC_H
#define X86_ATOMIC_H

#include <stdint.h>

void exchange(volatile uint64_t, uintptr_t newval);

#endif
