#ifndef CORE_H
#define CORE_H 1

#include <stdarg.h>
#include <stddef.h>

// string.c
// compiler needs these built-in functions
void*   memcpy(void* restrict dstptr, const void* restrict srcptr, size_t n);
void*   memmove(void* dstptr, const void* srcptr, size_t n);
int     memcmp(const void* aptr, const void* bptr, size_t n);
void*   memset(void* bufptr, int value, size_t n);
char*   strcpy(char* restrict dst, const char* restrict src);
int     strcmp(const char* s1, const char* s2);
size_t  strlen(const char* str);

// printf.h
int     snprintf(char* restrict buffer, size_t bufsz, const char* restrict format, ... );
int     vsnprintf(char* restrict buffer, size_t bufsz, const char* restrict format, va_list vlist );
int     printf(const char* restrict format, ... );

#endif
