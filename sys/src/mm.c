#include <mm.h>
#include <risx.h>
#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdmreq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 4
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmapreq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 4
};

void initpmm(void) {
    if (hhdmreq.response == NULL) panic("null hddm response.");
    if (memmapreq.response == NULL) panic("null memmap response.");

    initkpalloc(hhdmreq.response->offset, memmapreq.response);
}

void initvmm(void) {
    initkvalloc();    // I want this to be the virtual one...
}
