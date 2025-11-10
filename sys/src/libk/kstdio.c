#include "devices/serial.h"
#include "flanterm.h"
#include "kstdio.h"
#include "string.h"
#include <limits.h>
#include <stdarg.h>

extern struct flanterm_context* ctx;

int kputchar(char c) {
    serialputchar((int)c);
    flanterm_write(ctx, &c, 1);
    return (int)c;
}

int kputs(const char* str) {
    serialputs(str);
    flanterm_write(ctx, str, strlen(str));
    return 0;
}

void kfflush() {
    flanterm_flush(ctx);
}

static bool kprint(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++) {
        if (kputchar(bytes[i]) == EOF) {
            return false;
        }
    }

    return true;
}

static int kprintint(int xx, int base, int sign) {
    static char digits[] = "0123456789abcdef";
    char buf[16];
    int i;
    unsigned int x;

    if(sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';

    int count = i;
    while(--i >= 0)
        kputchar(buf[i]);
    return count;
}

int kprintf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%') {
                format++;
            }

            size_t amount = 1;
            while (format[amount] && format[amount] != '%') {
                amount++;
            }

            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!kprint(format, amount)) {
                return -1;
            }

            format = format + amount;
            written = written + amount;

            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;

            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!kprint(&c, sizeof(c))) {
                return -1;
            }

            written++;
        } else if (*format == 's') {
            format++;

            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!kprint(str, len)) {
                return -1;
            }

            written = written + len;
        } else if (*format == 'd') {
            format++;

            int val = va_arg(parameters, int);
            if (maxrem < 11) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            int cnt = kprintint(val, 10, 1);
            written = written + cnt;
        } else if (*format == 'u') {
            format++;

            unsigned int val = va_arg(parameters, unsigned int);
            if (maxrem < 10) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            int cnt = kprintint(val, 10, 0);
            written = written + cnt;
        } else if (*format == 'x') {
            format++;

            unsigned int val = va_arg(parameters, unsigned int);
            if (maxrem < 8) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            int cnt = kprintint(val, 16, 0);
            written = written + cnt;
        } else if (*format == 'p') {
            format++;

            unsigned int val = (unsigned int)va_arg(parameters, void*);
            if (maxrem < 8) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            int cnt = kprintint(val, 16, 0);
            written = written + cnt;
        } else {
            format = format_begun_at;

            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!kprint(format, len)) {
                return -1;
            }

            written = written + len;
            format = format + len;
        }
    }

    va_end(parameters);
    return written;
}

int kdebugf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    int result = kprintf(format, args);
    va_end(args);
    return result;
}

int kerrorf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    int result = kprintf(format, args);
    va_end(args);
    return result;
}
