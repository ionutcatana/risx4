// #include <io.h>
// #include <kernio.h>
#include "kstdio.h"
#include <stdbool.h>
#include <stddef.h>
// #include <stdlib.h>
#include <stdnoreturn.h>



noreturn void risx(void) {
    // kernel config variables
    // int mode = MODE_DEFAULT;

    // struct limine_framebuffer *fb = NULL;
    // if (framebuffer_request.response != NULL &&
    //     framebuffer_request.response->framebuffer_count >= 1) {
    //     fb = framebuffer_request.response->framebuffers[0];
    // }

    // initio(mode, fb);
    // kprintf("RISX Kernel.\n");
    // kprintf("Video:\n\t%dx%d\n\tpitch=%d\n\tbpp=%d\n",
    //         fb->width, fb->height, fb->pitch, fb->bpp);
    // kflush();

    // hang
    // TODO: not hang

    kprintf("Hello world!");
    kfflush();
    while (true);
}


