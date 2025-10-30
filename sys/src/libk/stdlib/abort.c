#include "libk/kernio.h"
#include "libk/stdlib.h"
#include <stdbool.h>
#include <stdnoreturn.h>

noreturn void abort(void) {
	kerrorf("panic: abort()\n");
    while (true);
}
