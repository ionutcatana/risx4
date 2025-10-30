#include "console.h"
#include "psf.h"
#include "framebuffer.h"
#include <stdint.h>

// Console state
uint32_t console_x = 0;
uint32_t console_y = 0;

void console_init(void) {
    // Clear screen to background color
    fb_clear(CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);
    console_x = 0;
    console_y = 0;
    fb_flip(); // Flip to show the cleared screen
}

static void console_scroll(void) {
    // Scroll up by one line
    for (uint32_t y = FONT_HEIGHT; y < SCREEN_HEIGHT; y++) {
        for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
            // Copy pixel from next line up
            uint32_t src_offset = (y * SCREEN_WIDTH + x) * (SCREEN_BPP / 8);
            uint32_t dst_offset = ((y - FONT_HEIGHT) * SCREEN_WIDTH + x) * (SCREEN_BPP / 8);

            back_buffer[dst_offset + 0] = back_buffer[src_offset + 0]; // B
            back_buffer[dst_offset + 1] = back_buffer[src_offset + 1]; // G
            back_buffer[dst_offset + 2] = back_buffer[src_offset + 2]; // R
        }
    }

    // Clear the bottom line
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

int kputchar(int c);


int console_putchar(int c) {
    char ch = (char)c;

    if (ch == '\n') {
        console_newline();
        fb_flip(); // Flip after newline
        return c;
    }

    if (ch == '\r') {
        console_x = 0;
        return c;
    }

    if (ch == '\t') {
        // Tab = 4 spaces
        for (int i = 0; i < 4; i++) {
            console_putchar(' ');
        }
        return c;
    }

    // Draw the character
    font_draw_char(console_x * FONT_WIDTH, console_y * FONT_HEIGHT, ch,
                   CONSOLE_FG_R, CONSOLE_FG_G, CONSOLE_FG_B,
                   CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);

    console_x++;

    // Wrap to next line if needed
    if (console_x >= CONSOLE_WIDTH) {
        console_newline();
    }

    fb_flip(); // Flip after each character for immediate display
    return c;
}
