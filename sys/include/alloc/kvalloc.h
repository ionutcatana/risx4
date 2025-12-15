#ifndef KVALLOC_H
#define KVALLOC_H 1

#include <limine.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// arch/x86/alloc/virt_allocator.c
void        initkvalloc(uint64_t physbase, uint64_t virtbase,
                        uint64_t offset, struct limine_memmap_response* memmap);
uintptr_t   allocptframe(void);
void        freepage(uintptr_t frameptr, size_t count);
uint64_t    offset(void);
void        mappage(uintptr_t l4addr,
                    uintptr_t va, uintptr_t pa, uint64_t flags);

// alloc/conversions.c
uintptr_t   physical(uintptr_t virtual);
uintptr_t   virtual(uintptr_t physical);

#endif
