#include "console.h"
#include "libk/kernio.h"

int kputchar(int ic) {
    return console_putchar(ic);
}
