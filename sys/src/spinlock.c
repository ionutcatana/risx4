#include <commonarch/interrupts.h>
#include <risx.h>
#include <spinlock.h>

#include <stdatomic.h>

void initlock(spinlock_t* lock, const char* name) {
    atomic_init(&lock->locked, LOCK_FREE);
    lock->name = name;
//  memset(&lock->pcs, 0, sizeof(lock->pcs));
}

void acquire(spinlock_t* lock) {
    pushinterrupts();
    if (atomic_load(&lock->locked) == LOCK_HELD)
        panic("attempting to acquire held lock.");
    
    while(atomic_exchange_explicit(&lock->locked, LOCK_HELD, memory_order_acquire) != LOCK_FREE);
}

void release(spinlock_t* lock) {
    if (atomic_load(&lock->locked) == LOCK_FREE)
        panic("attempting to release a free lock.");
    
    atomic_exchange_explicit(&lock->locked, LOCK_FREE, memory_order_release);
    popinterrupts();
}
