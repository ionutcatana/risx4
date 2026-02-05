#ifndef X86_REGISTERS_H
#define X86_REGISTERS_H 1

#include <stdint.h>

#define RFLAGS_IF       (1ull << 8)
#define CR0_PE          (1ull)
#define CR0_WP          (1ull << 15)
#define CR0_PG          (1ull << 30)
#define CR4_PSE         (1ull << 3)
#define CR4_PAE         (1ull << 4)
#define EFER_SYSCALL    (1ull)
#define EFER_LME        (1ull << 7)
#define EFER_NX         (1ull << 10)

// arch/x86/registers.S
uint64_t    readcr0(void);
uint64_t    readcr2(void);
uint64_t    readcr3(void);
uint64_t    readcr4(void);
uint64_t    readcr8(void);
uint64_t    readrflags(void);
uint64_t    readlapicid(void);

void        loadcr0(uint64_t value);
void        loadcr2(uint64_t value);
void        loadcr3(uint64_t value);
void        loadcr4(uint64_t value);
void        loadcr8(uint64_t value);
void        loadrflags(uint64_t value);
#endif
