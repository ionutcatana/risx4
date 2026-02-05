#ifndef PAGING_H
#define PAGING_H 1

#include <stdint.h>

#define PAGE_SIZE           (0x1000)
#define PAGE_SIZE_2M        (0x200000)
#define PAGE_SIZE_1G        (0x40000000)

#define STACK_BASE_VIRT     ((uintptr_t)0xffffffffffe00000)

#define LVL4_INDEX(va)      (((uintptr_t)(va) >> 39) & 0x1FF)
#define LVL3_INDEX(va)      (((uintptr_t)(va) >> 30) & 0x1FF)
#define LVL2_INDEX(va)      (((uintptr_t)(va) >> 21) & 0x1FF)
#define LVL1_INDEX(va)      (((uintptr_t)(va) >> 12) & 0x1FF)

#define OFFSET_MASK         ((uintptr_t)0x0000000000000fff)
#define SIGN_MASK           ((uintptr_t)0xffff000000000000)

#define OFFSET(va)          ((uintptr_t)(va) & OFFSET_MASK)
#define SIGN_EXTEND_POZ(va) ((uintptr_t)(va) & SIGN_MASK)
#define SIGN_EXTEND_NEG(va) ((uintptr_t)(va) | SIGN_MASK)

#define PT_NELEMENTS        (512)

typedef struct {
    uint64_t entries[PT_NELEMENTS];
} pagetable_t;

uintptr_t   readkernelpgtbl(void);

#endif
