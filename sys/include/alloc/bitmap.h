#ifndef BITMAP_H
#define BITMAP_H 1

#include <alloc/allocator.h>
#include <stdbool.h>
#include <stddef.h>

// alloc/bitmap.c
void    setbit(size_t idx);
void    unsetbit(size_t idx);
bool    checkbit(size_t idx);

#endif
