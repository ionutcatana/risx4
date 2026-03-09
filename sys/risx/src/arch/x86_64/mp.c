// note: having limine_mp_request here is not ideal
#include "arch/x86_64/specific/registers.h"
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

uint64_t cpucount(void) {
    return mpreq.response->cpu_count;
}

uint32_t bootstrapcpu(void) {
    return mpreq.response->bsp_lapic_id;
}

void enumeratecpus(void) {
    printf("bootstrap cpu lapic id %llu\n", bootstrapcpu());
    printf("%10s%15s\n", "lapic id", "processor id");
    for (uint64_t i = 0; i < cpucount(); i++)
        printf("%10lu%15lu\n", mpreq.response->cpus[i]->lapic_id, mpreq.response->cpus[i]->processor_id);
}

void initmp(void) {
    if (mpreq.response == NULL)
        return; // doesn't panic; systems may not have multiple processors

    enumeratecpus();

    for (uint64_t i = 0; i < mpreq.response->cpu_count; i++) {
        if (mpreq.response->cpus[i]->lapic_id == mpreq.response->bsp_lapic_id)
            continue;

        mpreq.response->cpus[i]->goto_address = &_start_application;
    }
}
