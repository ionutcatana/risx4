#include <stdint.h>
#ifndef BITMAP_H
#define BITMAP_H 1

#include <alloc/allocator.h>
#include <stdbool.h>
#include <stddef.h>

// alloc/bitmap.c
void    setbit(uint32_t* bitmap, size_t idx);
void    unsetbit(uint32_t* bitmap, size_t idx);
bool    checkbit(uint32_t* bitmap, size_t idx);

#endif
