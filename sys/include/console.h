#ifndef STDIO_H
#define STDIO_H 1

#include <stdint.h>

#define EOF (-1)

#define CONSOLE_WIDTH  (SCREEN_WIDTH / FONT_WIDTH)
#define CONSOLE_HEIGHT (SCREEN_HEIGHT / FONT_HEIGHT)
#define CONSOLE_FG_R   255 // White foreground
#define CONSOLE_FG_G   255
#define CONSOLE_FG_B   255
#define CONSOLE_BG_R   0   // Black background
#define CONSOLE_BG_G   0
#define CONSOLE_BG_B   0

extern uint32_t console_x;
extern uint32_t console_y;

void console_init(void);
int console_putchar(int c);

int kprintf(const char* restrict format, ...);
int kdebugf(const char* restrict format, ...);
int kerrorf(const char* restrict format, ...);
int kputchar(int c);
int kputs(const char* s);

#endif
