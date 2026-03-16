#ifndef SYNC_SEMAPHORE_H
#define SYNC_SEMAPHORE_H 1

#include <stdint.h>

struct semaphore {
    _Atomic uint64_t count;
};

void initsemaphore(struct semaphore* sem, uint64_t initial_count);
void proberen(struct semaphore* sem); // `wait`, `down`
void verhogen(struct semaphore* sem); // `signal`, `up`

#endif
