#include "limine.h"
#include "mm.h"
#include "panic.h"
#include "risx.h"
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdmreq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = LIMINE_API_REVISION
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmapreq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = LIMINE_API_REVISION
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request addrreq = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = LIMINE_API_REVISION
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_stack_size_request stacksizereq = {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision = LIMINE_API_REVISION,
    .stack_size = STACK_SIZE
};

void initpmm(void) {
    if (hhdmreq.response == NULL)
        panic("null hddm response.");

    if (memmapreq.response == NULL)
        panic("null memmap response.");


    initoffset(hhdmreq.response->offset);
    initkpalloc(memmapreq.response);
}

void initvmm(uint64_t stackbase) {
    if (addrreq.response == NULL)
        panic("null executable addr response.");

    if (stacksizereq.response == NULL)
        panic("null stack size response.");

    (void)stackbase;

    initkvalloc(addrreq.response->physical_base, addrreq.response->virtual_base, stackbase, memmapreq.response);
}
