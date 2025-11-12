#include <x86/gdt.h>

#include <stdint.h>

static union gdt_segment gdt[GDT_SIZE];
struct gdtr _desc; // used in gdt.S

void loadgdt(void);
void initgdt(void) {
    gdt[0].value = 0;
    gdt[1].value = 0x00CF9A000000FFFF;
    gdt[2].value = 0x00CF92000000FFFF;
    gdt[3].value = 0x00CFFA000000FFFF;
    gdt[4].value = 0x00CFF2000000FFFF;

    _desc.limit = sizeof(gdt) - 1;
    _desc.base = (uintptr_t)&gdt;

    loadgdt();
}
