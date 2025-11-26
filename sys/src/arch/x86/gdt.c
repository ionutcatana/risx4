#include <arch/x86/gdt.h>
#include <core.h>

#include <stdint.h>

static union gdtsegdesc_t gdt[GDT_SIZE];
struct gdtr_t _desc; // used in gdt.S
struct tss_t tss;

void loadgdt(void);
void initgdt(void) {
    gdt[0].value = 0;
    gdt[1].value = 0x00A09A000000FFFF;
    gdt[2].value = 0x00C092000000FFFF;
    gdt[3].value = 0x00C0F2000000FFFF;
    gdt[4].value = 0x00A0FA000000FFFF;

    _desc.limit = (GDT_SIZE) * sizeof(union gdtsegdesc_t) - 1;
    _desc.base = (uintptr_t)gdt;

    inittss();
    loadgdt();
    loadtr(TSS_SEG);
}

void loadtr(uint16_t selector);
void inittss(void) {
    memset(&tss, 0, sizeof(struct tss_t));
    tss.iomap_base = sizeof(struct tss_t);

    uintptr_t base = (uintptr_t)&tss;
    uint32_t limit = sizeof(struct tss_t) - 1;

    gdt[5].limit       = limit & 0xFFFF;
    gdt[5].base_lower  = base & 0xFFFF;
    gdt[5].base_middle = (base >> 16) & 0xFF;
    gdt[5].access      = 0x89;
    gdt[5].granularity = 0x00;
    gdt[5].base_upper  = (base >> 24) & 0xFF;
    gdt[6].limit       = (base >> 32) & 0xFFFF;
    gdt[6].base_lower  = (base >> 48) & 0xFFFF;
    gdt[6].base_middle = 0;
    gdt[6].access      = 0;
    gdt[6].granularity = 0;
    gdt[6].base_upper  = 0;
}


