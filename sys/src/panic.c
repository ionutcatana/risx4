#include <risx.h>

noreturn void abort(void);

noreturn void panic(const char* message) {
    printf("PANIC: %s\n", message);
    abort();
}
