#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#include <nanoprintf.h>
#include <serial.h>

#include <stdarg.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void serial_npf_putc(int c, void *ctx) {
    serialputchar(c);
}

int snprintf( char* restrict buffer, size_t bufsz, const char* restrict format, ... ) {
    va_list val;
    va_start(val, format);
    int const rv = npf_vsnprintf(buffer, bufsz, format, val);
    va_end(val);
    return rv;
}

int vsnprintf( char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist ) {
    int const rv = npf_vsnprintf(buffer, bufsz, format, vlist);
    return rv;
}

int printf( const char* restrict format, ... ) {
    va_list val;
    va_start(val, format);
    int const rv = npf_vpprintf(serial_npf_putc, NULL, format, val);
    va_end(val);
    return rv;
}

