#ifndef INTERRUPTS_H
#define INTERRUPTS_H 1

// arch/x86/interrupts.S
void enableinterrupts(void);
void disableinterrupts(void);

// arch/x86/interrupts.c
void pushinterrupts();
void popinterrupts();

#endif
