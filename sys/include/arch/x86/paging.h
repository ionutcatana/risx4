#ifndef X86_PAGING_H
#define X86_PAGING_H 1

#include <commonarch/paging.h>
#include <stdint.h>

#define PAGE_PRESENT            (1ULL)
#define PAGE_WRITABLE           (1ULL << 1)
#define PAGE_USER               (1ULL << 2)
#define PAGE_WRITE_THROUGH      (1ULL << 3)
#define PAGE_CACHE_DISABLE      (1ULL << 4)
#define PAGE_ACCESSED           (1ULL << 5)
#define PAGE_DIRTY              (1ULL << 6)
#define PAGE_HUGE               (1ULL << 7)
#define PAGE_GLOBAL             (1ULL << 8)
#define PAGE_NO_EXECUTE         (1ULL << 63)

#define PTE_PHYS_ADDR_MASK      (0x000FFFFFFFFFF000)

#define PAGE_MASK               (~(PAGE_SIZE - 1))
#define PAGE_ALIGN_DOWN(x)      ((x) & PAGE_MASK)
#define PAGE_ALIGN_UP(x)        (((x) + PAGE_SIZE - 1) & PAGE_MASK)

#define PTE_CREATE(phys, flags) ((uint64_t)(phys) | (uint64_t)(flags))
#define PTE_GET_ADDR(entry)     ((entry).value & PTE_PHYS_ADDR_MASK)
#define PTE_GET_FLAGS(entry)    ((entry).value & ~PTE_PHYS_ADDR_MASK)

#endif
