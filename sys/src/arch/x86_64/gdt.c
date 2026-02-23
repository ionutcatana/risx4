#include "arch/x86_64/specific/gdt.h"
#include "arch/x86_64/specific/registers.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "risx.h"

#include <stdint.h>

extern segdesc_t _gdt[69];
extern tss_t*    _tssaddrs[32];

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
    uint64_t limit = sizeof(tss_t) - 1;
    for (size_t i = 0; i < 32; i++) {
        // tss descriptors start at index 5, each is 2 entries
        size_t idx1 = 5 + (i * 2);
        size_t idx2 = 5 + (i * 2) + 1;
        uint64_t base = (uint64_t)_tssaddrs[i];

        _gdt[idx1].limit       = (limit & 0xFFFF);
        _gdt[idx1].base_lower  = (base & 0xFFFF);
        _gdt[idx1].base_middle = (base >> 16) & 0xFF;
        _gdt[idx1].access      = (0x89);
        _gdt[idx1].granularity = (0x00);
        _gdt[idx1].base_upper  = (base >> 24) & 0xFF;
        _gdt[idx2].limit       = (base >> 32) & 0xFFFF;
        _gdt[idx2].base_lower  = (base >> 48) & 0xFFFF;
        _gdt[idx2].base_middle = (0);
        _gdt[idx2].access      = (0);
        _gdt[idx2].granularity = (0);
        _gdt[idx2].base_upper  = (0);
    }
}

void inittss(void) {
    // initialize only what's needed for now
    for (size_t i = 0; i < 32; i++) {
        _tssaddrs[i]->iomap_base = 0;
        _tssaddrs[i]->rsp0 = 0;
    }
}



