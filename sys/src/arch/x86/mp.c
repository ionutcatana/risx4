#include <commonarch/mp.h>
#include <risx.h>
#include <limine.h>

#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mpreq = {
    .id = LIMINE_MP_REQUEST,
    .revision = 4
};

inline uint64_t cpucount() {
    return mpreq.response->cpu_count;
}

inline uint32_t bootstrapcpu() {
    return mpreq.response->bsp_lapic_id;
}

void enumeratecpus() {
    printf("bootstrap cpu lapic id %llu\n", bootstrapcpu());
    printf("%10s%15s%20s\n", "lapic id", "processor id", "goto address");
    for (uint64_t i = 0; i < cpucount(); i++)
        printf("%10lu%15lu%20lx\n",
               mpreq.response->cpus[i]->lapic_id,
               mpreq.response->cpus[i]->processor_id,
               mpreq.response->cpus[i]->goto_address);
}
