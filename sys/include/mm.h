#ifndef MM_H
#define MM_H 1

#include <stddef.h>
#include <stdint.h>

#define PAGESIZE    (0x1000)

// mm.c
void        initpmm(void);
void        initvmm(void);

// alloc/bitmap.c
uintptr_t   allocframe(size_t count);
void        freeframe(uintptr_t frameptr, size_t count);

#endif
