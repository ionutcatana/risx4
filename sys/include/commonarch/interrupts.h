#ifndef COMMONARCH_INTERRUPTS_H
#define COMMONARCH_INTERRUPTS_H 1

// arch/x86_64/interrupts.S
void    intenable(void);
void    intdisable(void);

// arch/x86_64/interrupts.c
void    intpush(void);
void    intpop(void);

#endif
