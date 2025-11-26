#include <atomic.h>
#include <core.h>
#include <interrupts.h>
#include <risx.h>
#include <spinlock.h>

#include <stdbool.h>

void initlock(struct spinlock* lock, const char* name) {
    lock->locked = false;
#if defined(RISXDEBUG)
    lock->name = name;
//  memset(&lock->pcs, 0, sizeof(lock->pcs));
#endif
}

void acquirelock(struct spinlock* lock) {
    pushinterrupts();
    if (holdinglock(lock)) {
        panic("attempting to acquire held lock.");
    }

    while(exchange(&lock->locked, LOCKHELD) != LOCKFREE);
}

void releaselock(struct spinlock* lock) {
    if (!holdinglock(lock)) {
        panic("attempting to release a free lock.");
    }

    exchange(&lock->locked, LOCKFREE);
    popinterrupts();
}

bool holdinglock(struct spinlock* lock) {
    bool result = lock->locked;
    return result;
}

