#ifndef MM_H
#define MM_H 1

// do not remove; all allocator functions are imported here and exposed using
// the mm.h header file
#include <alloc/allocator.h>

// mm.c
void    initpmm(void);
void    initvmm(void);

#endif
