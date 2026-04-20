#ifndef COMMONARCH_TIMER_H
#define COMMONARCH_TIMER_H 1

#include <stdint.h>

/* desired scheduler tick rate in Hz */
#define TIMER_HZ 100

// arch/x86_64/timer.c
void     inittimer(void);
uint64_t tickcount(void);

#endif
