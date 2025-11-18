#include <libk/kstdio.h>
#include <libk/kstdlib.h>
#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

noreturn void panic(const char* message) {
    kprintf("PANIC: %s\n", message);
    kfflush();
    abort();
}

noreturn void risx(void) {
    setup();

    kprintf("Entered RISX\nSetup successful\n");
    kfflush();

    panic("Unexpected return from scheduler");
}


