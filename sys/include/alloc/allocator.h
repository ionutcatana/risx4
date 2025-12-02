#ifndef ALLOCATOR_H
#define ALLOCATOR_H 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void        initalloc(void);
uintptr_t   allocframe(size_t count);
void        freeframe(uintptr_t frameptr, size_t count);

#endif
