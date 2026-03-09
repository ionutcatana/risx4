#ifndef COMMONARCH_MP_H
#define COMMONARCH_MP_H 1

#include "limine.h"
#include <stdint.h>

// arch/x86_64/mp.c
uint64_t cpucount(void);
uint32_t bootstrapcpu(void);
void     enumeratecpus(void);
void     initmp(void);

// arch/x86_64/start.S
void     _start_application(struct limine_mp_info* info);

#endif
