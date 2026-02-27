#ifndef ALLOC_KPALLOC_H
#define ALLOC_KPALLOC_H 1

#include "alloc/conversions.h"
#include "limine.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// alloc/kpalloc.c
void       initkpalloc(const struct limine_memmap_response* memmap);
physaddr_t allocframe(size_t count);
void       freeframe(physaddr_t frameptr, size_t count);

// alloc/hddmoffset.c
void     initoffset(const uint64_t offset);
uint64_t hhdmoffset(void);

// alloc/util.c
void     enumeratememmap(const struct limine_memmap_response* memmap);


#endif
