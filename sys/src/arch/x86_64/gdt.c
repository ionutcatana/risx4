#include "arch/x86_64/specific/gdt.h"
#include "arch/x86_64/specific/registers.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "risx.h"

#include <stdint.h>

extern uint64_t  _gdt[69];
extern segdesc_t _desc;
extern tss_t     _tss0;
extern tss_t*    tssaddrs[32];

void loadgdt(void);
void initgdt(void) {
    // the gdt is mostly static
    // but we need to fill in the TSS descriptors at runtime
    inittssdescriptors();
    loadgdt();
    loadtr(RISX_TSS0_SEG + (readlapicid() * 16));
}

void loadtr(uint16_t selector);
void inittssdescriptors(void) {
    uint16_t limit = sizeof(tss_t) - 1;
    for (size_t i = 0; i < 32; i++) {
        // tss descriptors start at index 5, each is 2 entries
        size_t idx1 = 5 + (i * 2);
        size_t idx2 = 5 + (i * 2) + 1;
        uint64_t base = (uint64_t)tssaddrs[i];
        _gdt[idx1] = ((base & 0xffff) << 16) | (limit & 0xffff) | 0x89;         // present, type=9 (available 64-bit tss)
        _gdt[idx2] = ((base >> 16) & 0xffffff) | ((base >> 24) & 0xff000000);   // high 32 bits of base
    }
}

void inittss(void) {
    // initialize only what's needed for now
    for (size_t i = 0; i < 32; i++) {
        tssaddrs[i]->iomap_base = 0;
        tssaddrs[i]->rsp0 = 0;
    }
}



