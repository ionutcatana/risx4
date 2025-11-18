#ifndef RISX_H
#define RISX_H

#include <stdnoreturn.h>

// setup.c
void setup(void);
// risx.c
noreturn void panic(const char* message);
noreturn void risx(void);

#endif
