#ifndef LIB_PRINTF_H
#define LIB_PRINTF_H 1

#include <stdarg.h>
#include <stddef.h>

// lib/printf.c
void initprintf(void);
int  snprintf(char* restrict buffer, size_t bufsz, const char* restrict format, ... );
int  vsnprintf(char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist );
int  printf(const char* restrict format, ... );

#endif
