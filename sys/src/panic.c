#include <libk/kstdio.h>
#include <libk/kstdlib.h>
#include <risx.h>

noreturn void panic(const char* message) {
    kprintf("PANIC: %s\n", message);
    abort();
}
