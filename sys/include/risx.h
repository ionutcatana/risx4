#ifndef RISX_H
#define RISX_H 1

#include <risx/config.h>
#include <risx/core.h>

#include <stdint.h>
#include <stdnoreturn.h>

// arch/x86/abort.S
noreturn void   abort(void);

// risx.c
noreturn void   panic(const char* message);
void            setup(uintptr_t stacktop);
noreturn void   risx(void);

#endif
