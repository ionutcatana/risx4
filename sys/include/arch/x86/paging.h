#ifndef X86_PAGING_H
#define X86_PAGING_H 1

#include <stdint.h>

#define PAGE_PRESENT            (1)
#define PAGE_WRITABLE           (1 << 1)
#define PAGE_USER               (1 << 2)
#define PAGE_WRITE_THROUGH      (1 << 3)
#define PAGE_CACHE_DISABLE      (1 << 4)
#define PAGE_ACCESSED           (1 << 5)
#define PAGE_DIRTY              (1 << 6)
#define PAGE_HUGE               (1 << 7)
#define PAGE_GLOBAL             (1 << 8)
#define PAGE_NO_EXECUTE         (1 << 63)

#define PTE_PHYS_ADDR_MASK      (0x000FFFFFFFFFF000)

#define PML4_INDEX(va)          (((uint64_t)(va) >> 39) & 0x1FF)
#define PDPT_INDEX(va)          (((uint64_t)(va) >> 30) & 0x1FF)
#define PD_INDEX(va)            (((uint64_t)(va) >> 21) & 0x1FF)
#define PT_INDEX(va)            (((uint64_t)(va) >> 12) & 0x1FF)

#define PAGE_SIZE               (0x1000)
#define PAGE_SIZE_2M            (0x200000)
#define PAGE_SIZE_1G            (0x40000000)

#define PAGE_MASK               (~(PAGE_SIZE - 1))
#define PAGE_ALIGN_DOWN(x)      ((x) & PAGE_MASK)
#define PAGE_ALIGN_UP(x)        (((x) + PAGE_SIZE - 1) & PAGE_MASK)

#define PTE_CREATE(phys, flags) ((uint64_t)(phys) | (uint64_t)(flags))
#define PTE_GET_ADDR(entry)     ((entry).value & PTE_PHYS_ADDR_MASK)
#define PTE_GET_FLAGS(entry)    ((entry).value & ~PTE_PHYS_ADDR_MASK)

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
        union {
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
            } __attribute__((packed));
            uint64_t flags : 12;
        };
        uint64_t phys_addr_fragment : 40; // access with (this << 12)
        uint64_t os_available_2 : 11;
        uint64_t no_execute : 1;
    };
    uint64_t value;
};

struct pagetable_t {
    union pagetable_entry_t entries[512];
} __attribute__((aligned(4096)));

#endif
