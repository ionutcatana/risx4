#include <arch/x86/interrupts.h>
#include <arch/x86/registers.h>

#include <stdint.h>

int64_t ninterrupts = 0;


void pushinterrupts() {
//  uint64_t rflags = readrflags();
    disableinterrupts();
    ninterrupts++;
}

void popinterrupts() {
    ninterrupts--;
    if (ninterrupts == 0) {
        enableinterrupts();
    }
    return;
}
