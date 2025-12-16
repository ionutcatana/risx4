#ifndef CONVERSIONS_H
#define CONVERSIONS_H 1

#include <stdint.h>

// alloc/conversions.c
uintptr_t   physical(uintptr_t virtual);
uintptr_t   virtual(uintptr_t physical);

#endif
