#ifndef COMMONARCH_INTERRUPTS_H
#define COMMONARCH_INTERRUPTS_H 1

#include <stddef.h>
#include <stdint.h>

// arch/x86_64/interrupts.h
struct trapframe;

// arch/x86_64/interrupts.S
void intenable(void);
void intdisable(void);
void istub(void);

// arch/x86_64/interrupts.c
void intpush(void);
void intpop(void);
void sethandler(size_t vector, uint64_t handler, uint8_t attributes, uint8_t ist);
void idispatch(struct trapframe* tf);

#endif
