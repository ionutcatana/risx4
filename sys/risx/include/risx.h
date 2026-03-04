#ifndef RISX_H
#define RISX_H 1

#include "risx/config.h"
#include <stdint.h>
#include <stdnoreturn.h>

// risx.c
void          setup(uint64_t stacktop);
noreturn void risx(void);

#endif
