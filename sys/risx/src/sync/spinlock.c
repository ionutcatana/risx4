#include "commonarch/interrupts.h"
#include "panic.h"
#include "risx.h"
#include "sync/spinlock.h"
#include <stdatomic.h>

void initlock(struct spinlock* lock, const char* name)
{
    atomic_init(&lock->locked, LOCK_FREE);
    lock->name = name;
//  memset(&lock->pcs, 0, sizeof(lock->pcs));
}

void acquire(struct spinlock* lock)
{
    intpush();
    while(atomic_exchange_explicit(&lock->locked, LOCK_HELD, memory_order_acquire) != LOCK_FREE);
}

void release(struct spinlock* lock)
{
    if (atomic_load(&lock->locked) == LOCK_FREE)
        panic("attempting to release a free lock.");

    atomic_exchange_explicit(&lock->locked, LOCK_FREE, memory_order_release);
    intpop();
}
