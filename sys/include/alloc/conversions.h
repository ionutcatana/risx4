#ifndef ALLOC_CONVERSIONS_H
#define ALLOC_CONVERSIONS_H 1

#include <stdint.h>

typedef uint64_t physaddr_t;
typedef void* virtaddr_t;

// alloc/conversions.c
physaddr_t   physical(virtaddr_t virtual);
virtaddr_t   virtual(physaddr_t physical);

#endif
