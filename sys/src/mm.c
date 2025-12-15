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

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request addrreq = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 4
};

void initpmm(void) {
    if (hhdmreq.response == NULL) panic("null hddm response.");
    if (memmapreq.response == NULL) panic("null memmap response.");

    initkpalloc(hhdmreq.response->offset, memmapreq.response);
}

void initvmm(void) {
    if (addrreq.response == NULL) panic("null executable addr response.");

    initkvalloc(addrreq.response->physical_base, addrreq.response->virtual_base,
                hhdmreq.response->offset, memmapreq.response);
}
