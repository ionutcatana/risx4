#ifndef SPINLOCK_H
#define SPINLOCK_H 1

#include <stdint.h>
#include <stdbool.h>

#define LOCKHELD    (1)
#define LOCKFREE    (0)

// TODO: stack trace; it requires a getter for the caller's PC/IP

#if defined(RISXDEBUG)
typedef struct {
    uint64_t locked;    // not `bool` because i want a register width value
    const char* name;
//  uintptr_t pcs[8];   // stack trace
} spinlock_t;
#else
typedef uint64_t spinlock_t;
#endif

void initlock(spinlock_t* lock, const char* name);
void acquire(spinlock_t* lock);
void release(spinlock_t* lock);
bool holding(spinlock_t* lock);

#endif
