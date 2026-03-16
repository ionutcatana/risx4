#ifndef RISX_H
#define RISX_H 1

#include "limine.h"
#include "risx/config.h"
#include <stdint.h>
#include <stdnoreturn.h>

struct pair {
    uint64_t value0; // stack top address
    uint64_t value1; // physical address of kernel page table
} __attribute__((packed));

// risx.c
void boostrap(void);
void setup(struct limine_mp_info* info);

// risx.c
noreturn void risx(void);

#endif
