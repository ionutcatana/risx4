#include <devices/serial.h>
#include <flanterm_backends/fb.h>
#include <flanterm.h>
#include <libk/kstdlib.h>
#include <limine.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>

// main.c
noreturn void risx(void);

// markers
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// requests
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(0);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_bootloader_info_request info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_cmdline_request cmdline_request = {
    .id = LIMINE_EXECUTABLE_CMDLINE_REQUEST,
    .revision = 0
};

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_memory_map_request memory_map_request = {
//     .id = LIMINE_MEMORY_MAP_REQUEST,
//     .revision = 0
// };

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_mp_request mp_request = {
//     .id = LIMINE_MP_REQUEST,
//     .revision = 0,
//     .flags = 0
// };

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

// __attribute__((used, section(".limine_requests")))
// static volatile struct limine_date_at_boot_request date_at_boot_request = {
//     .id = LIMINE_DATE_AT_BOOT_REQUEST,
//     .revision = 0
// };

struct flanterm_context* ctx = NULL;
int initconsole(struct limine_framebuffer* fb) {
    ctx = flanterm_fb_init(
        NULL,
        NULL,
        (uint32_t*)fb->address,
        fb->width,
        fb->height,
        fb->pitch,
        fb->red_mask_size,
        fb->red_mask_shift,
        fb->green_mask_size,
        fb->green_mask_shift,
        fb->blue_mask_size,
        fb->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );
    flanterm_set_autoflush(ctx, false);

    return 0;
}

noreturn void setup(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        abort();
    }

    initserial();

    if (framebuffer_request.response != NULL &&
        framebuffer_request.response->framebuffer_count >= 1) {
        initconsole(framebuffer_request.response->framebuffers[0]);
    }

    // get the ball rolling
    risx();
}
