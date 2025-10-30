#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include "multiboot2.h"

// Framebuffer information structure
struct framebuffer_info {
    uintptr_t addr;      // Framebuffer address
    uint32_t width;      // Width in pixels
    uint32_t height;     // Height in pixels
    uint32_t pitch;      // Bytes per scanline
    uint8_t bpp;         // Bits per pixel
    uint8_t type;        // Framebuffer type (RGB, etc.)
};

// Global framebuffer info (initialized from multiboot)
extern struct framebuffer_info fb_info;

// Initialize framebuffer from multiboot tag
void fb_init(struct multiboot2_tag_framebuffer* fb_tag);

// Put a pixel at (x, y) with RGB color
void fb_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

// Clear the entire framebuffer with a color
void fb_clear(uint8_t r, uint8_t g, uint8_t b);

#endif
