#include <arch/x86/gdt.h>
#include <arch/x86/registers.h>
#include <risx.h>

#include <stdint.h>

static gdtsegdesc_t gdt[NCPU][GDT_SIZE];
static tss_t        tss[NCPU];

void loadgdt(gdtr_t* desc);
void initgdt(void) {
    uint64_t const id = readlapicid();

    gdt[id][0].value = 0;
    gdt[id][1].value = 0x00A09A000000FFFF;
    gdt[id][2].value = 0x00C092000000FFFF;
    gdt[id][3].value = 0x00C0F2000000FFFF;
    gdt[id][4].value = 0x00A0FA000000FFFF;

    inittss(id);

    gdtr_t desc;
    desc.limit = (GDT_SIZE) * sizeof(gdtsegdesc_t) - 1;
    desc.base = (uintptr_t)&gdt[id];

    loadgdt(&desc);
    loadtr(TSS_SEG);
}

void loadtr(uint16_t selector);
void inittss(uint64_t id) {
    memset(&tss[id], 0, sizeof(tss_t));
    tss[id].iomap_base = sizeof(tss_t);

    uintptr_t base = (uintptr_t)&tss[id];
    uint32_t limit = sizeof(tss_t) - 1;

    gdt[id][5].limit       = limit & 0xFFFF;
    gdt[id][5].base_lower  = base & 0xFFFF;
    gdt[id][5].base_middle = (base >> 16) & 0xFF;
    gdt[id][5].access      = 0x89;
    gdt[id][5].granularity = 0x00;
    gdt[id][5].base_upper  = (base >> 24) & 0xFF;
    gdt[id][6].limit       = (base >> 32) & 0xFFFF;
    gdt[id][6].base_lower  = (base >> 48) & 0xFFFF;
    gdt[id][6].base_middle = 0;
    gdt[id][6].access      = 0;
    gdt[id][6].granularity = 0;
    gdt[id][6].base_upper  = 0;
}



