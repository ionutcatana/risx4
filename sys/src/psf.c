#include <stddef.h>
#include <stdint.h>
#include "psf.h"
#include "stdlib.h"
#include "framebuffer.h"

// Global font data
static uint8_t* font_bitmap = NULL;
static uint8_t font_width = 8;  // PSF fonts are 8 pixels wide
static uint8_t font_height = 16; // Hardcoded to 16 pixels for Terminus font

void font_init(uint8_t bitmap[], void* psf_header) {
    struct psf1_header* header = (struct psf1_header*)psf_header;
    if (header->magic != PSF1_MAGIC) {
        // only PSF1 is supported for now
        abort();
    }

    // Use hardcoded height instead of reading from header
    // font_height = header->char_size;
    font_bitmap = bitmap;

    size_t n = 256;
    if (header->mode & PSF1_MODE512) {
        n = 512;
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < font_height; j++) {
            bitmap[i * font_height + j] =
                *((uint8_t*)psf_header + sizeof(struct psf1_header) + i * font_height + j);
        }
    }
}

void access_bitmap() {
    // Placeholder function
}

uint8_t font_get_width(void) {
    return font_width;
}

uint8_t font_get_height(void) {
    return font_height;
}

void font_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_r, uint8_t fg_g, uint8_t fg_b, uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (font_bitmap == NULL || font_height == 0) {
        return; // Font not initialized
    }

    uint8_t char_index = (uint8_t)c;
    uint8_t* char_bitmap = &font_bitmap[char_index * font_height];

    for (uint8_t row = 0; row < font_height; row++) {
        uint8_t bitmap_byte = char_bitmap[row];

        for (uint8_t col = 0; col < font_width; col++) {
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
