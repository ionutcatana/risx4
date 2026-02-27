#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"
#include "commonarch/paging.h"
#include "limine.h"
#include "panic.h"
#include "risx.h"
#include <stddef.h>
#include <stdint.h>

#if defined(__x86_64__)
#include "arch/x86_64/paging.h"
#include "arch/x86_64/specific/registers.h"
#endif

extern char __kernel_start[];
extern char __kernel_end[];
extern char __kernel_size[];
extern char __text_start[];   // executable
extern char __text_end[];     // executable
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __data_end[];
extern char __bss_start[];
extern char __bss_end[];

static pagetable_t* kerneltable_addr = NULL;

pagetable_t* kerneltable() {
    return kerneltable_addr;
}

void initkvalloc(uint64_t base_physaddr, uint64_t base_virtaddr,
                 struct limine_memmap_response* memmap) {
    (void) base_physaddr;
    (void) base_virtaddr;
    (void) memmap;
    (void)__kernel_start;
    (void)__kernel_end;
    (void)__kernel_size;
    (void)__text_start;
    (void)__text_end;
    (void)__rodata_start;
    (void)__rodata_end;
    (void)__data_start;
    (void)__data_end;
    (void)__bss_start;
    (void)__bss_end;

    kerneltable_addr = virtual(allocframe(1));
    memset(kerneltable_addr, 0, sizeof(pagetable_t));
}

void unmappage(pagetable_t* globaltbl, uint64_t start_virtaddr, size_t count) {
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
