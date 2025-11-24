#include <arch/x86/atomic.h>
#include <spinlock.h>

#include <stdbool.h>

// TODO: interrupts safety: pushinterrupts, popinterrupts

void initlock(struct spinlock* lock, const char* name) {
    lock->locked = false;
#if defined(RISXDEBUG)
    lock->name = name;
#endif
}

void acquirelock(struct spinlock* lock) {
    while(exchange(&lock->locked, LOCKHELD) != LOCKFREE);
}

void releaselock(struct spinlock* lock) {
    exchange(&lock->locked, LOCKFREE);
}

bool holdinglock(struct spinlock* lock) {
    bool result = lock->locked;
    return result;
}

