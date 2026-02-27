#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"
#include "arch/x86_64/paging.h"
#include "arch/x86_64/specific/registers.h"
#include "commonarch/paging.h"
#include "limine.h"
#include "panic.h"
#include "risx.h"
#include <stddef.h>
#include <stdint.h>

static pagetable_t* kerneltable_addr = NULL;

pagetable_t* kerneltable() {
    return kerneltable_addr;
}

void initkvalloc(uint64_t base_physaddr, uint64_t base_virtaddr,
                 struct limine_memmap_response* memmap) {
    (void) base_physaddr;
    (void) base_virtaddr;
    (void) memmap;
}

void unmappage(uint64_t start_virtaddr, size_t count) {
    (void) start_virtaddr;
    (void) count;
}
void mappage(pagetable_t* globaltbl, uint64_t virtaddr,
                                     uint64_t physaddr, uint64_t flags) {
    (void) globaltbl;
    (void) virtaddr;
    (void) physaddr;
    (void) flags;
}
