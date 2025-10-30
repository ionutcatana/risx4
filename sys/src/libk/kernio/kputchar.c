#include "console.h"
#include "kernio.h"

int kputchar(int ic) {
    return console_putchar(ic);
}
