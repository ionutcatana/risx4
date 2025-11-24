#ifndef X86_REGISTERS_H
#define X86_REGISTERS_H 1

#include <stdint.h>

#define RFLAGS_IF   (uint64_t)0x0000000000000200
#define CR0_PE      (uint64_t)0x0000000000000001
#define CR0_WP      (uint64_t)0x0000000000010000
#define CR0_PG      (uint64_t)0x0000000080000000
#define CR4_PSE     (uint64_t)0x0000000000000010

// arch/x86/registers.S
uint64_t readcr0(void);
uint64_t readcr2(void);
uint64_t readcr3(void);
uint64_t readcr4(void);
uint64_t readcr8(void);
uint64_t readrflags(void);

void loadcr0(uint64_t value);
void loadcr2(uint64_t value);
void loadcr3(uint64_t value);
void loadcr4(uint64_t value);
void loadcr8(uint64_t value);
void loadrflags(uint64_t value);
#endif
