#ifndef SYNC_SEMAPHORE_H
#define SYNC_SEMAPHORE_H 1

#include <stdint.h>

typedef struct {
    _Atomic uint64_t count;
} semaphore_t;

void initsemaphore(semaphore_t* sem, uint64_t initial_count);
void proberen(semaphore_t* sem); // `wait`, `down`
void verhogen(semaphore_t* sem); // `signal`, `up`

#endif
