#ifndef RISX_H
#define RISX_H 1

#include <risx/config.h>
#include <risx/core.h>

#include <stdint.h>
#include <stdnoreturn.h>

// panic.c
noreturn void   abort(void);
noreturn void   panic(const char* message);

// setup.c
void            setup(uintptr_t stacktop);
noreturn void   risx(void);

#endif
