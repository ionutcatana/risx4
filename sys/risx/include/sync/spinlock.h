#ifndef SYNC_SPINLOCK_H
#define SYNC_SPINLOCK_H 1

#include <stdint.h>

#define LOCK_HELD   (1)
#define LOCK_FREE   (0)

struct spinlock {
    _Atomic uint64_t locked;    // 1 if locked, 0 if free
    const char* name;           // debugging
};

void initlock(struct spinlock* lock, const char* name);
void acquire(struct spinlock* lock);
void release(struct spinlock* lock);

#endif
