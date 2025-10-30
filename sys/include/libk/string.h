#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

// compiler needs these built-in functions
// libk/string/memcpy.c
void* memcpy(void* restrict s1, const void* restrict s2, size_t n);
// libk/string/memmove.c
void* memmove(void* s1, const void* s2, size_t n);
// libk/string/memcmp.c
int memcmp(const void* s1, const void* s2, size_t n);
// libk/string/memset.c
void* memset(void* s, int c, size_t n);
// libk/string/strcpy.c
char* strcpy(char* restrict s1, const char* restrict s2);
// libk/string/strcmp.c
int strcmp(const char* s1, const char* s2);
// libk/string/strlen.c
size_t strlen(const char* s);

#endif
