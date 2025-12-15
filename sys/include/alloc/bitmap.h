#ifndef BITMAP_H
#define BITMAP_H 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// alloc/bitmap.c
void    setbit(uint32_t* bitmap, size_t idx);
void    unsetbit(uint32_t* bitmap, size_t idx);
bool    checkbit(uint32_t* bitmap, size_t idx);

#endif
