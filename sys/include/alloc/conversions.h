#ifndef CONVERSIONS_H
#define CONVERSIONS_H 1

#include <stdint.h>

// alloc/conversions.c
uint64_t   physical(void* virtual);
void*       virtual(uint64_t physical);

#endif
