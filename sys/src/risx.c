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
                uint8_t* framebuffer = (uint8_t*)(uintptr_t)fb_tag->common.framebuffer_addr;

                for (size_t i = 0; i < fb_tag->common.framebuffer_height; i++) {
                    for (size_t j = 0; j < fb_tag->common.framebuffer_width; j++) {

                        size_t bytes_per_pixel = fb_tag->common.framebuffer_bpp / 8;
                        size_t pixel_offset = i * fb_tag->common.framebuffer_pitch + j * bytes_per_pixel;

                        for (size_t k = 0; k < bytes_per_pixel; k++) {
                            framebuffer[pixel_offset + k] = 0xFF;
                        }
                    }
                }

                // if (fb_tag->common.framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_RGB) {
                //     //
                // } else if (fb_tag->common.framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_INDEXED)  {
                //     //
                // } else {
                //     // I have no idea what EGA text mode is
                //     return 1;
                // }

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

