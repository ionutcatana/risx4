#include "console.h"
#include "framebuffer.h"
#include "libk/string.h"
#include "psf.h"
#include <stdint.h>

uint32_t console_x = 0;
uint32_t console_y = 0;

void console_init(void) {
    font_init((uintptr_t)&_binary_static_terminus_psf_start);
    fb_clear(CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);
    console_x = 0;
    console_y = 0;
    fb_flip();
}

static void console_scroll(void) {
    uint32_t bytes_per_pixel = (SCREEN_BPP / 8);
    uint32_t row_bytes = SCREEN_WIDTH * bytes_per_pixel;

    uint8_t *buf = back_buffer;

    for (uint32_t y = FONT_HEIGHT; y < SCREEN_HEIGHT; y++) {
        uint32_t src_offset = y * row_bytes;
        uint32_t dst_offset = (y - FONT_HEIGHT) * row_bytes;
        memcpy(&buf[dst_offset], &buf[src_offset], row_bytes);
    }

    for (uint32_t y = SCREEN_HEIGHT - FONT_HEIGHT; y < SCREEN_HEIGHT; y++) {
        for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
            fb_putpixel(x, y, CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);
        }
    }

    console_y--;
}

static void console_newline(void) {
    console_x = 0;
    console_y++;

    if (console_y >= CONSOLE_HEIGHT) {
        console_scroll();
    }
}

int console_putchar(int c) {
    char ch = (char)c;

    if (ch == '\n') {
        console_newline();
        fb_flip();
        return c;
    }

    if (ch == '\r') {
        console_x = 0;
        return c;
    }

    if (ch == '\t') {
        for (int i = 0; i < 4; i++) {
            console_putchar(' ');
        }
        return c;
    }

    font_draw_char(console_x * FONT_WIDTH, console_y * FONT_HEIGHT, ch,
                   CONSOLE_FG_R, CONSOLE_FG_G, CONSOLE_FG_B,
                   CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);

    console_x++;

    if (console_x >= CONSOLE_WIDTH) {
        console_newline();
    }

    fb_flip();
    return c;
}
