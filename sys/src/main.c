#include <libk/kstdio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

noreturn void risx(void) {
    kprintf("Hello world!");
    kfflush();
    while (true);
}


