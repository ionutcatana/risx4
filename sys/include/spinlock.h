#ifndef SPINLOCK_H
#define SPINLOCK_H 1

#include <stdint.h>
#include <stdbool.h>

#define LOCKHELD 1
#define LOCKFREE 0

struct spinlock {
    uint64_t locked; // not `bool` because i want it to be register width

#if defined(RISXDEBUG)
    const char* name;
#endif
};

void initlock(struct spinlock* lock, const char* name);
void acquirelock(struct spinlock* lock);
void releaselock(struct spinlock* lock);
bool holdinglock(struct spinlock* lock);

#endif
