#ifndef KVALLOC_H
#define KVALLOC_H 1

#include "commonarch/paging.h"
#include "limine.h"
#include <stddef.h>

// arch/x86_64/alloc/kvalloc.c
struct pagetable* kerneltable(void);
void              initkvalloc(uint64_t base_physaddr, uint64_t base_virtaddr, struct limine_memmap_response* memmap);
void              mappage(struct pagetable* globaltbl, uint64_t virtaddr, uint64_t physaddr, uint64_t flags);
void              unmappage(struct pagetable* globaltbl, uint64_t start_virtaddr, size_t count);
uint64_t*         walk(struct pagetable* pagetable, uint64_t virtaddr);

#endif
