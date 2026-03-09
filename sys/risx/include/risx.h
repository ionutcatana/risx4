#ifndef RISX_H
#define RISX_H 1

#include "limine.h"
#include "risx/config.h"
#include <stdint.h>
#include <stdnoreturn.h>

typedef struct {
    uint64_t value0; // stack top address
    uint64_t value1; // physical address of kernel page table
} __attribute__((packed)) pair_t;

// risx.c
pair_t  boostrap();
void    setup(struct limine_mp_info* info);
noreturn void risx(void);

#endif
