#ifndef RISX_H
#define RISX_H 1

#include "risx/config.h"
#include <stdint.h>
#include <stdnoreturn.h>

typedef struct {
    uint64_t value0; // stack top address
    uint64_t value1; // physical address of kernel page table
} pair_t;

// risx.c
void boostrap();
void setup();
noreturn void risx(void);

#endif
