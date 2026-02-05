#include <arch/x86/interrupts.h>
#include <arch/x86/registers.h>
#include <risx.h>

#include <stdint.h>

static int64_t  ninterrupts[NPROC] = {0};
static uint64_t intenabled[NPROC] = {0};

void pushinterrupts(void) {
    uint64_t rflags = readrflags();
    disableinterrupts();

    uint64_t id = readlapicid();
    if (ninterrupts[id] == 0)
        intenabled[id] = rflags & RFLAGS_IF;
    ninterrupts[id]++;
}

void popinterrupts(void) {
    uint64_t id = readlapicid();

    ninterrupts[id]--;
    if (ninterrupts[id] < 0)
        panic("decremented ninterrupts past 0.");

    if (readrflags() & RFLAGS_IF)
        panic("interrupts enabled flag is set.");

    if (ninterrupts[id] == 0 && intenabled[id])
        enableinterrupts();
}
