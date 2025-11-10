#include <libk/kstdio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

noreturn void risx(void) {
    kprintf("Entered RISX. Setup successful.\n");
    kfflush();
    while (true);
}


