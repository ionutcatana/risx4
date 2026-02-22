#ifndef ALLOC_CONVERSIONS_H
#define ALLOC_CONVERSIONS_H 1

#include <stdint.h>

// alloc/conversions.c
uint64_t   physical(void* virtual);
void*       virtual(uint64_t physical);

#endif
