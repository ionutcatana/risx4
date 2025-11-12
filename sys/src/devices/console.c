#include <flanterm_backends/fb.h>
#include <flanterm.h>
#include <libk/string.h>
#include <limine.h>

#include <stdbool.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct flanterm_context* ctx = NULL;
static bool console_initialized = false;

int initconsole(void) {
    struct limine_framebuffer* fb = NULL;
    if (framebuffer_request.response != NULL &&
        framebuffer_request.response->framebuffer_count >= 1) {
        fb = framebuffer_request.response->framebuffers[0];
    } else {
        return -1;
    }

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

    console_initialized = true;
    return 0;
}

void consoleputchar(int c) {
    if (!console_initialized) {
        return;
    }

    flanterm_write(ctx, (const char*)&c, 1);
}

void consoleputs(const char* str) {
    if (!console_initialized) {
        return;
    }

    flanterm_write(ctx, str, strlen(str));
}

void consoleflush(void) {
    if (!console_initialized) {
        return;
    }

    flanterm_flush(ctx);
}
