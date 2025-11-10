#include <libk/kstdio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

noreturn void halt(void);
noreturn void panic(const char* message) {
    kprintf("PANIC: %s\n", message);
    kfflush();
    halt();
}

noreturn void risx(void) {
    kprintf("Entered RISX\nSetup successful\n");
    kfflush();

    panic("Unexpected return from scheduler");
}


