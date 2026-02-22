#ifndef X86_PAGING_H
#define X86_PAGING_H 1

#include <commonarch/paging.h>
#include <stdint.h>

#define PAGE_PRESENT            (1ull)
#define PAGE_WRITABLE           (1ull << 1)
#define PAGE_USER               (1ull << 2)
#define PAGE_WRITE_THROUGH      (1ull << 3)
#define PAGE_CACHE_DISABLED     (1ull << 4)
#define PAGE_ACCESSED           (1ull << 5)
#define PAGE_DIRTY              (1ull << 6) // only for lowest level
#define PAGE_HUGE               (1ull << 7) // only for levels 2 and 3
#define PAGE_PAT                (1ull << 7) // only for lowest level
#define PAGE_GLOBAL             (1ull << 8) // only for lowest level
#define PAGE_NO_EXECUTE         (1ull << 63)

#define PTE_FLAGS_MASK          ((uint64_t)0x8000000000000fff)
#define PTE_ADDRESS_MASK_1G     ((uint64_t)0x0007ffffc0000000) // only level 3
#define PTE_ADDRESS_MASK_2M     ((uint64_t)0x0007ffffffe00000) // only level 2
#define PTE_ADDRESS_MASK        ((uint64_t)0x0007fffffffff000)

#endif
