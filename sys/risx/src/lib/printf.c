#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#include "commonarch/console.h"
#include "commonarch/serial.h"
#include "nanoprintf.h"
#include "sync/spinlock.h"
#include <stdarg.h>

static spinlock_t wrapperlk;
static spinlock_t snprintflk;
static spinlock_t vsnprintflk;
static spinlock_t printflk;

void initprintf(void) {
    initlock(&wrapperlk, "wrapper_npf_putc");
    initlock(&snprintflk, "npf_snprintf");
    initlock(&vsnprintflk, "npf_vsnprintf");
    initlock(&printflk, "npf_vpprintf");
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void wrapper_npf_putc(int c, void* ctx) {
    acquire(&wrapperlk);
    serialputchar(c);
    consoleputchar(c);
    release(&wrapperlk);
}

int snprintf(char* restrict buffer, size_t bufsz, const char* restrict format, ... ) {
    acquire(&snprintflk);
    va_list val;
    va_start(val, format);
    int const rv = npf_snprintf(buffer, bufsz, format, val);
    va_end(val);
    release(&snprintflk);

    return rv;
}

int vsnprintf(char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist ) {
    acquire(&vsnprintflk);
    int const rv = npf_vsnprintf(buffer, bufsz, format, vlist);
    release(&vsnprintflk);

    return rv;
}

int printf(const char* restrict format, ... ) {
    acquire(&printflk);
    va_list val;
    va_start(val, format);
    int const rv = npf_vpprintf(wrapper_npf_putc, NULL, format, val);
    va_end(val);
    release(&printflk);

    return rv;
}
