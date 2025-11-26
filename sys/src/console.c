#include <risx.h>
#include <console.h>
#include <flanterm_backends/fb.h>
#include <flanterm.h>
#include <limine.h>

#include <stdbool.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request limine_framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 4
};

struct flanterm_context* ft_ctx = NULL;

int initconsole(void) {
    if (limine_framebuffer_request.response == NULL ||
        limine_framebuffer_request.response->framebuffer_count == 0) {
        panic("invalid framebuffer response.");
    }

    struct limine_framebuffer fb;
    memcpy(
        &fb,
        // only one framebuffer supported for now
        limine_framebuffer_request.response->framebuffers[0],
        sizeof(struct limine_framebuffer));

    ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        fb.address, fb.width, fb.height, fb.pitch,
        fb.red_mask_size, fb.red_mask_shift,
        fb.green_mask_size, fb.green_mask_shift,
        fb.blue_mask_size, fb.blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );

    flanterm_set_autoflush(ft_ctx, true);

    return 0;
}

void consoleputchar(int c) {
    if (ft_ctx == NULL) {
        return;
    }

    flanterm_write(ft_ctx, (const char*)&c, 1);
}

