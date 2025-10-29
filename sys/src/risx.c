#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "multiboot2.h"
#include "stdlib.h"

uint32_t validate_mbi(uint32_t magic, uintptr_t addr) {
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        return 1;
    }

    if ((addr & 0x7) != 0) {
        return 1;
    }

    #define ALIGN_UP(value, align) (((value) + (align) - 1) & ~((align) - 1))

    uintptr_t tag_addr = addr + 8;
    struct multiboot2_tag* tag = (struct multiboot2_tag*)tag_addr;

    while (tag->type != MULTIBOOT2_TAG_TYPE_END) {
        switch (tag->type) {
            case MULTIBOOT2_TAG_TYPE_CMDLINE:
                break;

            case MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME:
                break;

            case MULTIBOOT2_TAG_TYPE_MODULE:
                break;

            case MULTIBOOT2_TAG_TYPE_BASIC_MEMINFO:
                break;

            case MULTIBOOT2_TAG_TYPE_BOOTDEV:
                break;

            case MULTIBOOT2_TAG_TYPE_MMAP:
                break;

            case MULTIBOOT2_TAG_TYPE_FRAMEBUFFER:
                struct multiboot2_tag_framebuffer* fb_tag = (struct multiboot2_tag_framebuffer*)tag;
                uintptr_t framebuffer = (uintptr_t)fb_tag->common.framebuffer_addr;

                if (
                    fb_tag->common.framebuffer_type != MULTIBOOT2_FRAMEBUFFER_TYPE_RGB ||
                    fb_tag->common.framebuffer_bpp != 24
                ) {
                    // other video modes and bit depths are not supported
                    return 1;
                }

                // fill the screen with white color
                struct color {
                    uint8_t blue;
                    uint8_t green;
                    uint8_t red;
                } color = {0xFF, 0xFF, 0xFF};

                for (size_t i = 0; i < fb_tag->common.framebuffer_height; i++) {
                    for (size_t j = 0; j < fb_tag->common.framebuffer_width; j++) {
                        struct color* pixel = (struct color*)(framebuffer + i * fb_tag->common.framebuffer_pitch + j * (fb_tag->common.framebuffer_bpp / 8));
                        *pixel = color;
                    }
                }

                // draw a diagonal line in red color
                color = (struct color){.blue = 0x00, .green = 0x00, .red = 0xFF};

                for (size_t i = 0; i < fb_tag->common.framebuffer_height && i < fb_tag->common.framebuffer_width; i++) {
                    struct color* pixel = (struct color*)(framebuffer + i * fb_tag->common.framebuffer_pitch + i * (fb_tag->common.framebuffer_bpp / 8));
                    *pixel = color;
                }

                break;
        }

        tag_addr += ALIGN_UP(tag->size, 8);
        tag = (struct multiboot2_tag*)tag_addr;
    }

    #undef ALIGN_UP

    return 0;
}

noreturn void risx(uint32_t magic, uintptr_t addr) {
    if (validate_mbi(magic, addr) != 0) {
        abort();
    }

    while (true);
}

