#include <arch/x86/interrupts.h>
#include <arch/x86/registers.h>
#include <risx.h>

#include <stdint.h>

static int64_t ninterrupts = 0;
static uint64_t intenabled = 1;

void pushinterrupts(void) {
    uint64_t rflags = readrflags();
    disableinterrupts();

    ninterrupts++;
    if (ninterrupts == 0) {
        intenabled = rflags & RFLAGS_IF;
    }
}

void popinterrupts(void) {
    ninterrupts--;
    if (ninterrupts < 0) {
        panic("decremented ninterrupts past 0.");
    }

    if (readrflags() & RFLAGS_IF) {
        panic("interrupts enabled flag is set.");
    }

    if (ninterrupts == 0) {
        enableinterrupts();
    }
}
