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

typedef union {
    struct {
        uintptr_t offset : 12;
        uintptr_t l1i : 9;
        uintptr_t l2i : 9;
        uintptr_t l3i : 9;
        uintptr_t l4i : 9;
        uintptr_t sign_ext : 16;
    } __attribute__((packed));
    uintptr_t value;
} virtaddr_t;

// pages larger than 4K
typedef union {
    struct {
        uintptr_t offset : 21;
        uintptr_t l2i : 9;
        uintptr_t l3i : 9;
        uintptr_t l4i : 9;
        uintptr_t sign_ext : 16;
    } __attribute__((packed));
    uintptr_t value;
} virtaddr_2m_t;

typedef union {
    struct {
        uintptr_t offset : 30;
        uintptr_t l3i : 9;
        uintptr_t l4i : 9;
        uintptr_t sign_ext : 16;
    } __attribute__((packed));
    uintptr_t value;
} virtaddr_1g_t;

#endif
