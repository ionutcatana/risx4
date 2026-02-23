#include "arch/x86_64/specific/gdt.h"
#include "arch/x86_64/specific/registers.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "risx.h"

#include <stdint.h>

extern uint64_t  _gdt[69];
extern segdesc_t _desc;
extern tss_t     _tss0;

void loadgdt(void);
void initgdt(void) {
    uint64_t const id = readlapicid();
    inittss(id);
    loadgdt();
    loadtr(RISX_TSS0_SEG + (id * 16));
}

void loadtr(uint16_t selector);
void inittss(uint64_t id) {
    // memset(&tss[id], 0, sizeof(tss_t));
    // tss[id].iomap_base = sizeof(tss_t);

    uint64_t base = (uint64_t)&tss[id];
    uint32_t limit = sizeof(tss_t) - 1;

    // gdt[id][5].limit       = limit & 0xFFFF;
    // gdt[id][5].base_lower  = base & 0xFFFF;
    // gdt[id][5].base_middle = (base >> 16) & 0xFF;
    // gdt[id][5].access      = 0x89;
    // gdt[id][5].granularity = 0x00;
    // gdt[id][5].base_upper  = (base >> 24) & 0xFF;
    // gdt[id][6].limit       = (base >> 32) & 0xFFFF;
    // gdt[id][6].base_lower  = (base >> 48) & 0xFFFF;
    // gdt[id][6].base_middle = 0;
    // gdt[id][6].access      = 0;
    // gdt[id][6].granularity = 0;
    // gdt[id][6].base_upper  = 0;
}



