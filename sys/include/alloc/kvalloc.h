#ifndef KVALLOC_H
#define KVALLOC_H 1

#include "commonarch/paging.h"
#include "limine.h"
#include <stddef.h>

// arch/x86_64/alloc/virt_allocator.c
void initkvalloc(uint64_t physbase, uint64_t virtbase,
                 struct limine_memmap_response* memmap);

void unmappage(uint64_t frameptr, size_t count);
void mappage(pagetable_t* globaltbl, uint64_t va, uint64_t pa, uint64_t flags);

#endif
