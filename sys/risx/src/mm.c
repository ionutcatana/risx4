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

void initpmm(void)
{
    if (hhdmreq.response == NULL)
        panic("null hddm response.");

    if (memmapreq.response == NULL)
        panic("null memmap response.");


    initoffset(hhdmreq.response->offset);
    initkpalloc(memmapreq.response);
}

void initvmm()
{
    if (addrreq.response == NULL)
        panic("null executable addr response.");

    initkvalloc(addrreq.response->physical_base, addrreq.response->virtual_base, memmapreq.response);
}
