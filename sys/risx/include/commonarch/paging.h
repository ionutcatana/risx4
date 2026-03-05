#ifndef COMMONARCH_PAGING_H
#define COMMONARCH_PAGING_H 1

#include <stdbool.h>
#include <stdint.h>

#define PAGE_SIZE           (0x1000)

#define STACK_BASE_VIRT     ((uint64_t)0xffffffffffe00000)

#define LVL4_INDEX(va)      (((uint64_t)(va) >> 39) & 0x1ff)
#define LVL3_INDEX(va)      (((uint64_t)(va) >> 30) & 0x1ff)
#define LVL2_INDEX(va)      (((uint64_t)(va) >> 21) & 0x1ff)
#define LVL1_INDEX(va)      (((uint64_t)(va) >> 12) & 0x1ff)

#define OFFSET_MASK         ((uint64_t)0x0000000000000fff)
#define SIGN_MASK           ((uint64_t)0xffff000000000000)

#define OFFSET(va)          ((uint64_t)(va) & OFFSET_MASK)
#define SIGN_EXTEND_POZ(va) ((uint64_t)(va) & SIGN_MASK)
#define SIGN_EXTEND_NEG(va) ((uint64_t)(va) | SIGN_MASK)

#define NENTRIES_PT         (512)

struct {
    uint64_t entries[NENTRIES_PT];
} pagetable;

// arch/x86_64/alloc/virt_allocator.c
uint64_t readkernelpgtbl(void);

// TODO: unimplemented yet
uint64_t walk(struct pagetable* pagetable, uint64_t virtaddr, bool create);

#endif
