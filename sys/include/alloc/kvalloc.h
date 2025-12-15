#ifndef KVALLOC_H
#define KVALLOC_H 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void        initkvalloc(uint64_t physbase, uint64_t virtbase);
uintptr_t   allocpage(size_t count);
void        freepage(uintptr_t frameptr, size_t count);

#endif
