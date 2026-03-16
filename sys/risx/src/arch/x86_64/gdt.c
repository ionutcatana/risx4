#include "arch/x86_64/specific/gdt.h"
#include "arch/x86_64/specific/registers.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "memlayout.h"
#include "risx.h"
#include <stdint.h>

static struct gdtr desc __attribute__((aligned(16)));

// from arch/x86_64/gdt.S
extern segdesc_t _gdt[NENTRIES_GDT];
extern struct tss* _tssaddrs[NCPU];

// from arch/x86_64/interrupts.S
extern uint64_t  _istacks[NCPU];

void initgdt(void)
{
    desc.limit = sizeof(segdesc_t) * NENTRIES_GDT - 1;
    desc.base = (uint64_t)_gdt;

    // the gdt is mostly static
    // but we need to fill in the TSS descriptors at runtime
    inittssdescriptors();
    loadgdt(&desc);
    loadtr(RISX_TSS0_SEG + (readlapicid() * 2 * sizeof(segdesc_t)));
}

void inittssdescriptors(void)
{
    uint64_t limit = sizeof(struct tss) - 1;
    for (size_t i = 0; i < NCPU; i++) {
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

void inittss(void)
{
    // initialize only what's needed for now
    for (size_t i = 0; i < NCPU; i++) {
        _tssaddrs[i]->iomap_base = sizeof(struct tss);
        _tssaddrs[i]->rsp0 = _istacks[i] + STACK_SIZE;
        _tssaddrs[i]->ist[0] = _istacks[i] + STACK_SIZE;
    }
}



