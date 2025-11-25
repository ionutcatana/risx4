#ifndef X86_PAGING_H
#define X86_PAGING_H 1

#include <stdint.h>

union virtaddr_t {
    struct {
        uintptr_t offset : 12;
        uintptr_t l1i : 9;
        uintptr_t l2i : 9;
        uintptr_t l3i : 9;
        uintptr_t l4i : 9;
        uintptr_t sign_ext : 16;
    } __attribute__((packed));
    uintptr_t value;
};

union pagetable_entry_t {
    struct {
        uint64_t present : 1;
        uint64_t writable : 1;
        uint64_t user_accessible : 1;
        uint64_t write_through : 1;
        uint64_t cache_disabled : 1;
        uint64_t accessed : 1;
        uint64_t dirty : 1;
        uint64_t huge_page : 1;
        uint64_t global : 1;
        uint64_t os_available : 3;
        uint64_t phys_addr_fragment : 40; // access with (this << 12)
        uint64_t os_available_2 : 11;
        uint64_t no_execute : 1;
    } __attribute__((packed));
    uint64_t value;
};

struct pagetable_t {
    union pagetable_entry_t entries[512];
} __attribute__((aligned(4096)));

#endif
