#include <libk/kstdio.h>
#include <risx.h>

noreturn void halt(void);
noreturn void abort(void) {
    halt();
}

noreturn void panic(const char* message) {
    kprintf("PANIC: %s\n", message);
    abort();
}
