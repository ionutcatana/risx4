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
}

static void console_scroll(void) {
    uint8_t char_height = font_get_height();

    // Scroll up by one line
    for (uint32_t y = char_height; y < fb_info.height; y++) {
        for (uint32_t x = 0; x < fb_info.width; x++) {
            // Copy pixel from next line up
            uint32_t src_offset = (y * fb_info.pitch) + (x * (fb_info.bpp / 8));
            uint32_t dst_offset = ((y - char_height) * fb_info.pitch) + (x * (fb_info.bpp / 8));

            uint8_t* src_pixel = (uint8_t*)(fb_info.addr + src_offset);
            uint8_t* dst_pixel = (uint8_t*)(fb_info.addr + dst_offset);

            dst_pixel[0] = src_pixel[0]; // B
            dst_pixel[1] = src_pixel[1]; // G
            dst_pixel[2] = src_pixel[2]; // R
        }
    }

    // Clear the bottom line
    for (uint32_t y = fb_info.height - char_height; y < fb_info.height; y++) {
        for (uint32_t x = 0; x < fb_info.width; x++) {
            fb_putpixel(x, y, CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);
        }
    }

    console_y--;
}

static void console_newline(void) {
    console_x = 0;
    console_y++;

    uint8_t char_height = font_get_height();
    if (console_y >= (fb_info.height / char_height)) {
        console_scroll();
    }
}

int kputchar(int c);


int console_putchar(int c) {
    char ch = (char)c;

    if (ch == '\n') {
        console_newline();
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
    uint8_t char_width = font_get_width();
    uint8_t char_height = font_get_height();

    font_draw_char(console_x * char_width, console_y * char_height, ch,
                   CONSOLE_FG_R, CONSOLE_FG_G, CONSOLE_FG_B,
                   CONSOLE_BG_R, CONSOLE_BG_G, CONSOLE_BG_B);

    console_x++;

    // Wrap to next line if needed
    if (console_x >= CONSOLE_WIDTH) {
        console_newline();
    }

    return c;
}
