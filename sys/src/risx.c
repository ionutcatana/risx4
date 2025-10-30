#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "multiboot2.h"
#include "stdlib.h"
#include "framebuffer.h"
#include "psf.h"
#include "console.h"

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

                if (
                    fb_tag->common.framebuffer_type != MULTIBOOT2_FRAMEBUFFER_TYPE_RGB ||
                    fb_tag->common.framebuffer_bpp != 24
                ) {
                    // other video modes and bit depths are not supported
                    return 1;
                }

                // Initialize framebuffer
                fb_init(fb_tag);

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

    font_init((uintptr_t)&_binary_static_terminus_psf_start);
    console_init();

    kputs("RISX Kernel booted successfully!\n");
    kputs("Framebuffer console initialized.\n");
    kprintf("Resolution: %dx%d\n", fb_info.width, fb_info.height);

    for (int i = 0; i < 300; i++) {
        kprintf("Counting: %d\n", i);
    }

    while (true);
}

