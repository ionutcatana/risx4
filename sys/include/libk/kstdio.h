#ifndef KSTDIO_H
#define KSTDIO_H 1

#define EOF (-1)

// libk/kstdio.h
int kprintf(const char* restrict format, ...);
int kdebugf(const char* restrict format, ...);
int kerrorf(const char* restrict format, ...);
int kputchar(char c);
int kputs(const char* s);
void kfflush(void);

#endif
