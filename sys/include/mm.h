#ifndef MM_H
#define MM_H 1

#include <stdint.h>

#define HHOFFSET    (0xffffffff80000000)

union virtaddr_t {
    struct {
        uintptr_t reserved : 16;    // must contain the same bit as #47
        uintptr_t l4i : 9;
        uintptr_t l3i : 9;
        uintptr_t l2i : 9;
        uintptr_t l1i : 9;
        uintptr_t offset : 12;
    };
    uintptr_t value;
} __attribute__((packed));

// mm.c
void initmm(void);
void initpmm(void);

#endif
