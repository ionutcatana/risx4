#include "console.h"
#include "framebuffer.h"
#include "libk/kernio.h"
#include "libk/stdlib.h"
#include "libk/string.h"
#include "multiboot2.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

uintptr_t mem_lo;
uintptr_t mem_hi;
char bootloader_name[20];
char commands[200];

uint32_t parse_mbi(uint32_t magic, uintptr_t addr) {
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
                struct multiboot2_tag_string* cmdline_tag = (struct multiboot2_tag_string*)tag;
                strcpy(commands, cmdline_tag->string);
                break;

            case MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME:
                struct multiboot2_tag_string* name_tag = (struct multiboot2_tag_string*)tag;
                strcpy(bootloader_name, name_tag->string);
                break;

            case MULTIBOOT2_TAG_TYPE_MODULE:
                break;

            case MULTIBOOT2_TAG_TYPE_BASIC_MEMINFO:
                struct multiboot2_tag_basic_meminfo* basic_meminfo_tag = (struct multiboot2_tag_basic_meminfo*)tag;
                mem_lo = basic_meminfo_tag->mem_lower;
                mem_hi = basic_meminfo_tag->mem_upper;

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
    if (parse_mbi(magic, addr) != 0) {
        abort();
    }

    console_init();

    kprintf("RISX booted.\n");

    kprintf("Framebuffer:\n");
    kprintf("- Resolution: %dx%d\n", fb_info.width, fb_info.height);
    kprintf("- Pitch: %u\n", fb_info.pitch);
    kprintf("- Depth: %u\n", fb_info.bpp);
    kprintf("- Address: 0x%x\n", fb_info.addr);
    kprintf("Basic memory info:\n");
    kprintf("- Lo: %d KiB\n- Hi: %d KiB\n", mem_lo, mem_hi);
    kprintf("Bootloader: %s\n", bootloader_name);

    if (strlen(commands) == 0) {
        kprintf("No command line arguments provided.\n");
    } else {
        kprintf("Command line: %s\n", commands);
    }

    while (true);
}

