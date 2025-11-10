#include <flanterm_backends/fb.h>
#include <flanterm.h>
#include <libk/string.h>
#include <limine.h>

#include <stdbool.h>

struct flanterm_context* ctx = NULL;
static bool serial_initialized = false;

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

    serial_initialized = true;
    return 0;
}

void consoleputchar(int c) {
    if (!serial_initialized) {
        return;
    }

    flanterm_write(ctx, (const char*)&c, 1);
}

void consoleputs(const char* str) {
    if (!serial_initialized) {
        return;
    }

    flanterm_write(ctx, str, strlen(str));
}

void consoleflush() {
    flanterm_flush(ctx);
}
