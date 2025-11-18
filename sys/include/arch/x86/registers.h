#ifndef X86_REGISTERS_H
#define X86_REGISTERS_H 1

#include <stdint.h>

// arch/x86/registers.S
uint64_t readcr0(void);
uint64_t readcr2(void);
uint64_t readcr3(void);
uint64_t readcr4(void);
uint64_t readcr8(void);

#endif
