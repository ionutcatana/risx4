#include <commonarch/atomic.h>
#include <commonarch/mp.h>
#include <risx.h>
#include <limine.h>

#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mpreq = {
    .id = LIMINE_MP_REQUEST,
    .revision = 4
};

uint64_t cpucount() {
    return mpreq.response->cpu_count;
}

uint32_t bootstrapcpu() {
    return mpreq.response->bsp_lapic_id;
}

void enumeratecpus() {
    printf("bootstrap cpu lapic id %u\n", bootstrapcpu());
    for (uint64_t i = 0; i < cpucount(); i++)
        printf("lapic id %u -> processor id %u\n",
                mpreq.response->cpus[i]->lapic_id,
                mpreq.response->cpus[i]->processor_id);
}
