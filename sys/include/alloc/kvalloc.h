#ifndef KVALLOC_H
#define KVALLOC_H 1

#include <limine.h>
#include <commonarch/paging.h>

#include <stddef.h>

// arch/x86/alloc/virt_allocator.c
void        initkvalloc(uint64_t physbase, uint64_t virtbase,
                        struct limine_memmap_response* memmap);
void        unmappage(uintptr_t frameptr, size_t count);
void        mappage(pagetable_t* globaltbl,
                    uintptr_t va, uintptr_t pa, uint64_t flags);

#endif
