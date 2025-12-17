#ifndef CONVERSIONS_H
#define CONVERSIONS_H 1

#include <stdint.h>

// alloc/conversions.c
uintptr_t   physical(void* virtual);
void*       virtual(uintptr_t physical);

#endif
