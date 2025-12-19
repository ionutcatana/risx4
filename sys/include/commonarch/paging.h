#ifndef PAGING_H
#define PAGING_H 1

#include <stdint.h>

#define PAGE_SIZE       (0x1000)
#define PAGE_SIZE_2M    (0x200000)
#define PAGE_SIZE_1G    (0x40000000)

#define LVL4_INDEX(va)  (((uintptr_t)(va) >> 39) & 0x1FF)
#define LVL3_INDEX(va)  (((uintptr_t)(va) >> 30) & 0x1FF)
#define LVL2_INDEX(va)  (((uintptr_t)(va) >> 21) & 0x1FF)
#define LVL1_INDEX(va)  (((uintptr_t)(va) >> 12) & 0x1FF)

#define SIGN_EXTEND(va) ()

#endif
