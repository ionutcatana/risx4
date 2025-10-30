#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include "multiboot2.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 24
#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define BACK_BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * (SCREEN_BPP / 8))

struct framebuffer_info {
    uintptr_t addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
    uint8_t type;
};

extern struct framebuffer_info fb_info;
extern uint8_t back_buffer[BACK_BUFFER_SIZE];

void fb_init(struct multiboot2_tag_framebuffer* fb_tag);
void fb_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void fb_clear(uint8_t r, uint8_t g, uint8_t b);
void fb_flip(void);

#endif
