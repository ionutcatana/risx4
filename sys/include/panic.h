#ifndef PANIC_H
#define PANIC_H 1

#include <stdnoreturn.h>

noreturn void panic(const char* message);

#endif
