#ifndef STDIO_H
#define STDIO_H 1

#include <stdint.h>

#define EOF (-1)

// Console configuration
#define CONSOLE_WIDTH   80  // Characters per line
#define CONSOLE_HEIGHT  25  // Lines on screen
#define CONSOLE_FG_R   255 // White foreground
#define CONSOLE_FG_G   255
#define CONSOLE_FG_B   255
#define CONSOLE_BG_R   0   // Black background
#define CONSOLE_BG_G   0
#define CONSOLE_BG_B   0

// Initialize console with framebuffer
void console_init(void);

// Console cursor position
extern uint32_t console_x;
extern uint32_t console_y;

// Internal console functions
void console_init(void);
int console_putchar(int c);

int kprintf(const char* restrict format, ...);
int kdebugf(const char* restrict format, ...);
int kerrorf(const char* restrict format, ...);
int kputchar(int c);
int kputs(const char* s);

#endif
