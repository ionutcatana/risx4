#ifndef SYSCALL_H
#define SYSCALL_H 1

#include <stdint.h>
#include <arch/x86/trapframe.h>

void initsyscalldiscovery(void);
void init_syscall_msrs(void);
void syscall_handler(trapframe_t* tf);

#endif
