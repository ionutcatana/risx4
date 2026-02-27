#ifndef X86_64_EXTENDED_PAGING_H
#define X86_64_EXTENDED_PAGING_H 1

#include <stdint.h>

/* common flags */
#define PAGE_PRESENT            (1ull)       // 0 = ignored, 1 = present
#define PAGE_WRITABLE           (1ull << 1)  // 0 = read-only
#define PAGE_USER               (1ull << 2)  // 0 = supervisor, 1 = user
#define PAGE_WRITE_THROUGH      (1ull << 3)  // useful for memory-mapped I/O
#define PAGE_CACHE_DISABLED     (1ull << 4)  // useful for memory-mapped I/O
#define PAGE_ACCESSED           (1ull << 5)  // for clearing
#define PAGE_NO_EXECUTE         (1ull << 63) // 0 = executable, 1 = not executable

/* only for levels 3 and 2*/
#define PAGE_HUGE               (1ull << 7)  // *unused*

/* only for level 1 */
#define PAGE_DIRTY              (1ull << 6) // for clearing
#define PAGE_PAT                (1ull << 7) // *unused*
#define PAGE_GLOBAL             (1ull << 8) // 1 = not flushed on context switch

#define PTE_FLAGS_MASK          ((uint64_t)0x8000000000000fff)
#define PTE_ADDRESS_MASK        ((uint64_t)0x0007fffffffff000)

/* every architecture should define these constants */
#define PAGE_RISX_CODE          (PAGE_PRESENT | PAGE_WRITABLE | PAGE_GLOBAL)
#define PAGE_RISX_DATA          (PAGE_PRESENT | PAGE_WRITABLE | PAGE_GLOBAL | PAGE_NO_EXECUTE)
#define PAGE_USER_CODE          (PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER)
#define PAGE_USER_DATA          (PAGE_PRESENT | PAGE_WRITABLE | PAGE_USER | PAGE_NO_EXECUTE)

#endif
