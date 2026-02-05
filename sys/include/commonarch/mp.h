#ifndef MP_H
#define MP_H 1

#include <stdint.h>

// arch/x86/mp.c
uint64_t    cpucount();
uint32_t    bootstrapcpu(); // bsp_lapic_id on x86
void        enumeratecpus();
void        initmp(void);

#endif
