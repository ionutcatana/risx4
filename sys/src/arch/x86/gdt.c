#include <x86/gdt.h>

#include <stdint.h>

static union gdt_segment gdt[GDT_SIZE];
struct gdtr _desc; // used in gdt.S

void loadgdt(void);
void initgdt(void) {
    gdt[0].value = 0;
    gdt[1].value = 0x00E09A0000000000;
    gdt[2].value = 0x00E0920000000000;
    gdt[3].value = 0x00E0FA0000000000;
    gdt[4].value = 0x00E0F20000000000;

    _desc.limit = (GDT_SIZE) * sizeof(union gdt_segment) - 1;
    _desc.base = (uintptr_t)gdt;

    loadgdt();
}
