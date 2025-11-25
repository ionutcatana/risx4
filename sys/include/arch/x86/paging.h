#ifndef X86_PAGING_H
#define X86_PAGING_H 1

#include <stdint.h>

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


#endif
