#include <arch/x86/paging.h>
#include <commonarch/paging.h>
#include <mm.h>
#include <risx.h>
#include <spinlock.h>

typedef size_t dev_t;
typedef size_t ino_t;
typedef size_t off_t;
static spinlock_t malloc_global_mutex;

static inline int heap_initial_lock(spinlock_t* lock) {
    initlock(lock, "malloc_lock");
    return 0;
}

static inline int heap_acquire_lock(spinlock_t* lock) {
    acquire(lock);
    return 0;
}

static inline void heap_release_lock(spinlock_t* lock) {
    release(lock);
}

static uintptr_t heap_start      = KERNEL_HEAP_BASE;
static uintptr_t heap_break      = KERNEL_HEAP_BASE;
static uintptr_t heap_mapped_top = KERNEL_HEAP_BASE;
static spinlock_t heap_sbrk_lock;

uintptr_t readkernelpgtbl(void);
static void* sbrk(intptr_t increment) {
    if (heap_start == 0) initheap();

    acquire(&heap_sbrk_lock);

    uintptr_t old_break = heap_break;
    uintptr_t new_break = heap_break + increment;
    if (increment > 0) {
        /* overflow */
        if (new_break < old_break) {
            release(&heap_sbrk_lock);
            return (void*)-1;
        }

        if (new_break > heap_mapped_top) {
             uintptr_t current_pt = readkernelpgtbl() + hhdmoffset();
             uintptr_t needed = new_break - heap_mapped_top;
             size_t    frames = (needed + PAGE_SIZE - 1) / PAGE_SIZE;

             for (size_t i = 0; i < frames; i++) {
                 uintptr_t phys = allocframe(1);
                 /* oom, just fail */
                 if (phys == 0) {
                     release(&heap_sbrk_lock);
                     return (void*)-1;
                 }

                 mappage((void*)current_pt, heap_mapped_top, phys, PAGE_PRESENT | PAGE_WRITABLE | PAGE_NO_EXECUTE);
                 memset((void*)heap_mapped_top, 0x5A5A5A5A, PAGE_SIZE); // fill with junk

                 heap_mapped_top += PAGE_SIZE;
             }
        }
    } else if (increment < 0) {
        /* underflow */
        if (new_break < heap_start) {
             release(&heap_sbrk_lock);
             return (void*)-1;
        }
    }

    heap_break = new_break;
    release(&heap_sbrk_lock);
    return (void*)old_break;
}

#define LACKS_ERRNO_H
#define LACKS_FCNTL_H
#define LACKS_SCHED_H
#define LACKS_STDLIB_H
#define LACKS_STRING_H
#define LACKS_STRINGS_H
#define LACKS_SYS_MMAN_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_TYPES_H
#define LACKS_TIME_H
#define LACKS_UNISTD_H
#define ABORT                           panic("malloc internal abort")
#define HAVE_MMAP                       0
#define HAVE_MREMAP                     0
#define MALLOC_FAILURE_ACTION           ((void)0)
#define NO_MALLOC_STATS                 1
#define NO_PTHREADS                     1
#define USE_LOCKS                       2
#define USE_SPIN_LOCKS                  1
#define EINVAL                          22
#define ENOMEM                          12
#define MLOCK_T                         spinlock_t
#define INITIAL_LOCK(lock)              heap_initial_lock(lock)
#define ACQUIRE_LOCK(lock)              heap_acquire_lock(lock)
#define RELEASE_LOCK(lock)              heap_release_lock(lock)
#define TRY_LOCK(lock)                  (0)
#define ACQUIRE_MALLOC_GLOBAL_LOCK()    heap_acquire_lock(&malloc_global_mutex)
#define RELEASE_MALLOC_GLOBAL_LOCK()    heap_release_lock(&malloc_global_mutex)
#define MORECORE                        sbrk
#define MORECORE_CONTIGUOUS             1
#define MORECORE_CANNOT_TRIM            1
#include <dlmalloc.inc>

void initheap(void) {
    initlock(&heap_sbrk_lock, "sbrk");
    initlock(&malloc_global_mutex, "malloc_global");
    heap_start = KERNEL_HEAP_BASE;
    heap_break = KERNEL_HEAP_BASE;
    heap_mapped_top = KERNEL_HEAP_BASE;
}
