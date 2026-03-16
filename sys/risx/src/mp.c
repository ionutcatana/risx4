#include "commonarch/mp.h"
#include "commonarch/paging.h"
#include "limine.h"
#include "risx.h"
#include "lib/printf.h"
#include <stddef.h>
#include <stdint.h>
#include "alloc/kvalloc.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mpreq = {
    .id = LIMINE_MP_REQUEST,
    .revision = LIMINE_API_REVISION
};

uint64_t cpucount(void)
{
    return mpreq.response->cpu_count;
}

uint32_t bootstrapcpu(void)
{
#if defined (__x86_64__)
    return mpreq.response->bsp_lapic_id;
#endif
}

void enumeratecpus(void)
{
    printf("processors:\n");
    printf("%8s%13s\n", "lapic id", "processor id");
    for (uint64_t i = 0; i < cpucount(); i++)
        printf("%8lu%13lu %s\n", mpreq.response->cpus[i]->lapic_id, mpreq.response->cpus[i]->processor_id, mpreq.response->cpus[i]->processor_id == bootstrapcpu() ? "(!)" : "");
}

void initmp(void)
{
    if (mpreq.response == NULL)
        return; // doesn't panic; systems may not have multiple processors

//  enumeratecpus();

    struct limine_mp_info* bootstrapcpu_info;
    for (uint64_t i = 0; i < cpucount(); i++) {
        if (mpreq.response->cpus[i]->lapic_id == bootstrapcpu()) {
            bootstrapcpu_info = mpreq.response->cpus[i];
            continue;
        }

        mpreq.response->cpus[i]->goto_address = &_start_application;
    }

    _start_application(bootstrapcpu_info);
}
