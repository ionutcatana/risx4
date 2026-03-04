#ifndef SYNC_SPINLOCK_H
#define SYNC_SPINLOCK_H 1

#include <stdint.h>

#define LOCK_HELD   (1)
#define LOCK_FREE   (0)

typedef struct {
    _Atomic uint64_t locked;    // 1 if locked, 0 if free
    const char* name;           // debugging
} spinlock_t;

void initlock(spinlock_t* lock, const char* name);
void acquire(spinlock_t* lock);
void release(spinlock_t* lock);

#endif
