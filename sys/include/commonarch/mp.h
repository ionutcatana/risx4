#ifndef MP_H
#define MP_H 1

#include <stdint.h>

// arch/x86/mp.c
static inline uint64_t  cpucount();
static inline uint32_t  bootstrapcpu(); // bsp_lapic_id on x86
void                    enumeratecpus();

#endif
