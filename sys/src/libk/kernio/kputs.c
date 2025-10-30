#include "console.h"
#include "kernio.h"

int kputs(const char* string) {
    int count = 0;
    while (*string) {
        console_putchar(*string++);
        count++;
    }
    return count;
}
