#include "libk/stdlib.h"

int abs(int j) {
    if (j < 0) {
        j = -1 * j;
    }

    return j;
}
