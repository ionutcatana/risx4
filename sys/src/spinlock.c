#include <spinlock.h>

#include <stdbool.h>

void initlock(struct spinlock* lock, const char* name) {
    lock->locked = false;
#if defined(RISXDEBUG)
    lock->name = name;
#endif
}

void acquirelock(struct spinlock* lock) {
    return;
}

void releaselock(struct spinlock* lock) {
    return;
}

bool holdinglock(struct spinlock* lock) {
    return true;
}

