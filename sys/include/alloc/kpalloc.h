#ifndef KPALLOC_H
#define KPALLOC_H 1

#include <limine.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// alloc/phys_allocator.c
void        initkpalloc(const uint64_t offset,
                        const struct limine_memmap_response* memmap);
uintptr_t   allocframe(size_t count);
void        freeframe(uintptr_t frameptr, size_t count);

// alloc/util.c
void        enumeratememmap(const struct limine_memmap_response* memmap);


#endif
