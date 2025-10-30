#include <stdbool.h>
#include <stdnoreturn.h>
#include "kernio.h"
#include "stdlib.h"

noreturn void abort(void) {
	kerrorf("panic: abort()\n");
    while (true);
}
