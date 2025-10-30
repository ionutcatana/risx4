#include <stddef.h>
#include <stdint.h>
#include "psf.h"
#include "stdlib.h"
#include "framebuffer.h"

static uint8_t* font_bitmap = NULL;

void font_init(uint8_t bitmap[], void* psf_header) {
    struct psf1_header* header = (struct psf1_header*)psf_header;
    if (
        header->magic != PSF1_MAGIC ||
        header->mode & PSF1_MODE512
    ) {
        // only PSF1 fonts with 256 characters are supported for now
        abort();
    }

    font_bitmap = bitmap;

    size_t n = 256;
    if (header->mode & PSF1_MODE512) {
        n = 512;
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < FONT_HEIGHT; j++) {
            bitmap[i * FONT_HEIGHT + j] =
                *((uint8_t*)psf_header + sizeof(struct psf1_header) + i * FONT_HEIGHT + j);
        }
    }
}

void font_draw_char(size_t x, size_t y, char c,
                    uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                    uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (font_bitmap == NULL) {
        return; // Font not initialized
    }

    uint8_t char_index = (uint8_t)c;
    uint8_t* char_bitmap = &font_bitmap[char_index * FONT_HEIGHT];

    for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
        uint8_t bitmap_byte = char_bitmap[row];

        for (uint8_t col = 0; col < FONT_WIDTH; col++) {
            uint8_t bit = (bitmap_byte >> (7 - col)) & 1;

            if (bit) {
                // Foreground pixel
                fb_putpixel(x + col, y + row, fg_r, fg_g, fg_b);
            } else {
                // Background pixel
                fb_putpixel(x + col, y + row, bg_r, bg_g, bg_b);
            }
        }
    }
}
