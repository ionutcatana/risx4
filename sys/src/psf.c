#include "framebuffer.h"
#include "libk/stdlib.h"
#include "psf.h"
#include <stddef.h>
#include <stdint.h>

static uint8_t* font_bitmap = NULL;

void font_init(uintptr_t psf1_header_addr) {
    struct psf1_header* header = (struct psf1_header*)psf1_header_addr;

    // only PSF1 fonts with 256 characters are supported for now
    if (header->magic != PSF1_MAGIC || header->mode & PSF1_MODE512) {
        abort();
    }

    font_bitmap = (uint8_t*)(psf1_header_addr + sizeof(struct psf1_header));
}

void font_draw_char(size_t x, size_t y, char c,
                    uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                    uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    if (font_bitmap == NULL) {
        abort();
    }

    uint8_t char_index = (uint8_t)c;
    uint8_t* char_bitmap = &font_bitmap[char_index * FONT_HEIGHT];

    for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
        uint8_t bitmap_byte = char_bitmap[row];

        for (uint8_t col = 0; col < FONT_WIDTH; col++) {

            if ((bitmap_byte >> (7 - col)) & 1) {
                fb_putpixel(x + col, y + row, fg_r, fg_g, fg_b);
            } else {
                fb_putpixel(x + col, y + row, bg_r, bg_g, bg_b);
            }
        }
    }
}
