#ifndef BITMAP_H
#define BITMAP_H 1

#include <stdbool.h>
#include <stddef.h>

void    setbit(size_t idx);
void    unsetbit(size_t idx);
bool    checkbit(size_t idx);

#endif
