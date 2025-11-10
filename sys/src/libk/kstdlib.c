#include <kstdlib.h>
#include <stdbool.h>
#include <stdnoreturn.h>

noreturn void abort(void) {
    while (true);
}

int abs(int j) {
    if (j < 0) {
        j = -1 * j;
    }

    return j;
}
