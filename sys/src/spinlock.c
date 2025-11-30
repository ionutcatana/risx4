#include <atomic.h>
#include <interrupts.h>
#include <risx.h>
#include <spinlock.h>

void initlock(spinlock_t* lock, const char* name) {
#if defined(RISXDEBUG)
    lock->locked = 1;
    lock->name = name;
//  memset(&lock->pcs, 0, sizeof(lock->pcs));
#else
    lock = 1;
#endif
}

void acquire(spinlock_t* lock) {
    pushinterrupts();
    if (holding(lock)) {
        panic("attempting to acquire held lock.");
    }

    while(exchange(&lock->locked, LOCKHELD) != LOCKFREE);
}

void release(spinlock_t* lock) {
    if (!holding(lock)) {
        panic("attempting to release a free lock.");
    }

    exchange(&lock->locked, LOCKFREE);
    popinterrupts();
}

bool holding(spinlock_t* lock) {
    bool result = lock->locked;
    return result;
}

