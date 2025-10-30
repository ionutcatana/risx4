#include "framebuffer.h"
#include "libk/stdlib.h"
#include "libk/string.h"
#include "multiboot2.h"
#include <stdint.h>

struct framebuffer_info fb_info;
uint8_t back_buffer[BACK_BUFFER_SIZE];

void fb_init(struct multiboot2_tag_framebuffer* fb_tag) {
    fb_info.addr = (uintptr_t)fb_tag->common.framebuffer_addr;
    fb_info.width = fb_tag->common.framebuffer_width;
    fb_info.height = fb_tag->common.framebuffer_height;
    fb_info.pitch = fb_tag->common.framebuffer_pitch;
    fb_info.bpp = fb_tag->common.framebuffer_bpp;
    fb_info.type = fb_tag->common.framebuffer_type;
}

void fb_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        abort();
    }

    uint32_t offset = (y * SCREEN_WIDTH + x) * (SCREEN_BPP / 8);
    back_buffer[offset + 0] = b; // Blue
    back_buffer[offset + 1] = g; // Green
    back_buffer[offset + 2] = r; // Red
}

void fb_clear(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (uint32_t x = 0; x < SCREEN_WIDTH; x++) {
            fb_putpixel(x, y, r, g, b);
        }
    }
}

void fb_flip(void) {
    memcpy((void*)fb_info.addr, back_buffer, BACK_BUFFER_SIZE);
}
