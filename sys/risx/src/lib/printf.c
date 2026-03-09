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

static spinlock_t wrapperlock;
static spinlock_t snprintflock;
static spinlock_t vsnprintflock;
static spinlock_t printflock;

void initprintf(void) {
    initlock(&wrapperlock, "wrapperlock");
    initlock(&snprintflock, "snprintf");
    initlock(&vsnprintflock, "vsnprintf");
    initlock(&printflock, "printf");
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void wrapper_npf_putc(int c, void* ctx) {
    acquire(&wrapperlock);
    serialputchar(c);
    consoleputchar(c);
    release(&wrapperlock);
}

int snprintf(char* restrict buffer, size_t bufsz, const char* restrict format, ... ) {
    acquire(&snprintflock);
    va_list val;
    va_start(val, format);
    int const rv = npf_vsnprintf(buffer, bufsz, format, val);
    va_end(val);
    release(&snprintflock);

    return rv;
}

int vsnprintf(char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist ) {
    acquire(&vsnprintflock);
    int const rv = npf_vsnprintf(buffer, bufsz, format, vlist);
    release(&vsnprintflock);

    return rv;
}

int printf(const char* restrict format, ... ) {
    acquire(&printflock);
    va_list val;
    va_start(val, format);
    int const rv = npf_vpprintf(wrapper_npf_putc, NULL, format, val);
    va_end(val);
    release(&printflock);

    return rv;
}
