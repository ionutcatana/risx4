#ifndef STDIO_H
#define STDIO_H 1

#include <stdint.h>

#define CONSOLE_WIDTH (SCREEN_WIDTH / FONT_WIDTH)
#define CONSOLE_HEIGHT (SCREEN_HEIGHT / FONT_HEIGHT)
#define CONSOLE_FG_R ((uint8_t)0xAA)
#define CONSOLE_FG_G ((uint8_t)0xAA)
#define CONSOLE_FG_B ((uint8_t)0xAA)
#define CONSOLE_BG_R ((uint8_t)0x0)
#define CONSOLE_BG_G ((uint8_t)0x0)
#define CONSOLE_BG_B ((uint8_t)0x0)

extern uint32_t console_x;
extern uint32_t console_y;

void console_init(void);
int console_putchar(int c);


#endif
