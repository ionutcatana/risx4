#ifndef KERNIO_H
#define KERNIO_H 1

#define EOF (-1)

// libk/kernio/kprintf.c
int kprintf(const char* restrict format, ...);
int kdebugf(const char* restrict format, ...);
int kerrorf(const char* restrict format, ...);
// libk/kernio/kputchar.c
int kputchar(int c);
// libk/kernio/kputs.c
int kputs(const char* s);

#endif
