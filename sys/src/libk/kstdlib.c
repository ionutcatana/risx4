#include <libk/kstdlib.h>

#include <stdbool.h>
#include <stdnoreturn.h>

noreturn void halt(void);
noreturn void abort(void) {
    halt();
}

int abs(int j) {
    if (j < 0) {
        j = -1 * j;
    }

    return j;
}
