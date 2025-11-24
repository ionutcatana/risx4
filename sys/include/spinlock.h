#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdbool.h>

struct spinlock {
    bool locked;

#if defined(RISXDEBUG)
    const char* name;
#endif
};

void initlock(struct spinlock* lock, const char* name);
void acquirelock(struct spinlock* lock);
void releaselock(struct spinlock* lock);
bool holdinglock(struct spinlock* lock);

#endif
