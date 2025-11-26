#include <core.h>
#include <risx.h>

noreturn void halt(void);
noreturn void abort(void) {
    halt();
}

noreturn void panic(const char* message) {
    printf("PANIC: %s\n", message);
    abort();
}
