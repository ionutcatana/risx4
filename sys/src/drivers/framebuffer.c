#include "framebuffer.h"
#include "multiboot2.h"
#include <stdint.h>

// Global framebuffer info
struct framebuffer_info fb_info;

void fb_init(struct multiboot2_tag_framebuffer* fb_tag) {
    fb_info.addr = (uintptr_t)fb_tag->common.framebuffer_addr;
    fb_info.width = fb_tag->common.framebuffer_width;
    fb_info.height = fb_tag->common.framebuffer_height;
    fb_info.pitch = fb_tag->common.framebuffer_pitch;
    fb_info.bpp = fb_tag->common.framebuffer_bpp;
    fb_info.type = fb_tag->common.framebuffer_type;
}

void fb_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= fb_info.width || y >= fb_info.height) {
        return; // Out of bounds
    }

    // Assuming RGB format (24 bpp)
    uint32_t offset = y * fb_info.pitch + x * (fb_info.bpp / 8);
    uint8_t* pixel = (uint8_t*)(fb_info.addr + offset);

    pixel[0] = b; // Blue
    pixel[1] = g; // Green
    pixel[2] = r; // Red
}

void fb_clear(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t y = 0; y < fb_info.height; y++) {
        for (uint32_t x = 0; x < fb_info.width; x++) {
            fb_putpixel(x, y, r, g, b);
        }
    }
}
