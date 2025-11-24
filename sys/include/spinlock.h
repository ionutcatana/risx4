#ifndef SPINLOCK_H
#define SPINLOCK_H 1

#include <stdint.h>
#include <stdbool.h>

#define LOCKHELD    (1)
#define LOCKFREE    (0)

// TODO: stack trace; it requires a getter for the caller's PC/IP

struct spinlock {
    uint64_t locked;    // not `bool` because i want a register width value

#if defined(RISXDEBUG)
    const char* name;
//  uintptr_t pcs[8];   // stack trace
#endif
};

void initlock(struct spinlock* lock, const char* name);
void acquirelock(struct spinlock* lock);
void releaselock(struct spinlock* lock);
bool holdinglock(struct spinlock* lock);

#endif
