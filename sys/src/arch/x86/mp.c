// note: having limine_mp_request here is not ideal
#include <arch/x86/registers.h>
#include <commonarch/mp.h>
#include <commonarch/paging.h>
#include <limine.h>
#include <risx.h>

#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

void initgdt(void);
void loadidt(void);
noreturn void enterrisx(uintptr_t stack_top);

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

void mpentrypoint(struct limine_mp_info *info) {
    loadcr3(readkernelpgtbl());
    initgdt();
    loadidt();

    uint64_t stack_top = STACK_BASE_VIRT + ((info->processor_id + 1) * STACK_SIZE);
    enterrisx(stack_top);
}

void initmp(void) {
    if (mpreq.response == NULL) return; // doesn't panic; systems may not have multiple processors
    for (uint64_t i = 0; i < mpreq.response->cpu_count; i++) {
        if (mpreq.response->cpus[i]->lapic_id == mpreq.response->bsp_lapic_id) continue;
        atomic_store_explicit((_Atomic limine_goto_address *)&mpreq.response->cpus[i]->goto_address,
                              (limine_goto_address)mpentrypoint,
                              memory_order_seq_cst);
    }
}
