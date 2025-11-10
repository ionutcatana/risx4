#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

// compiler needs these built-in functions
// libk/string.c
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t n);
void* memmove(void* dstptr, const void* srcptr, size_t n);
int memcmp(const void* aptr, const void* bptr, size_t n);
void* memset(void* bufptr, int value, size_t n);
char* strcpy(char* restrict dst, const char* restrict src);
int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);

#endif
