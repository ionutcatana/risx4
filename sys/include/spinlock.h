#ifndef SPINLOCK_H
#define SPINLOCK_H 1

#include <stdint.h>
#include <stdbool.h>

#define LOCK_HELD   (1)
#define LOCK_FREE   (0)

typedef struct {
    _Atomic(uint64_t) locked;   // not `bool` because i want a register width value
    const char* name;
//  uintptr_t pcs[8];           // stack trace
} spinlock_t;

void    initlock(spinlock_t* lock, const char* name);
void    acquire(spinlock_t* lock);
void    release(spinlock_t* lock);

#endif
