#include <x86/gdt.h>

#include <stdint.h>

static union gdt_segment gdt[GDT_SIZE];
struct gdtr _desc; // used in gdt.S

void loadgdt(void);
void initgdt(void) {
    gdt[0].value = 0;

    uint64_t kernel_code = 0;
    kernel_code |= 0xd << 8;    // type of selector
    kernel_code |= 1 << 12;     // not a system descriptor
    kernel_code |= 0 << 13;     // dpl field = 0
    kernel_code |= 1 << 15;     // present
    kernel_code |= 1 << 21;     // long-mode segment
    gdt[1].value = kernel_code << 32;

    uint64_t kernel_data = 0;
    kernel_data |= 0x2 << 8;    // type of selector
    kernel_data |= 1 << 12;     // not a system descriptor
    kernel_data |= 0 << 13;     // dpl field = 0
    kernel_data |= 1 << 15;     // present
    kernel_data |= 1 << 21;     // long-mode segment
    gdt[2].value = kernel_data << 32;

    uint64_t user_code = kernel_code | (3 << 13);
    gdt[3].value = user_code;

    uint64_t user_data = kernel_data | (3 << 13);
    gdt[4].value = user_data;

    _desc.limit = (GDT_SIZE) * sizeof(union gdt_segment) - 1;
    _desc.base = (uintptr_t)gdt;

    loadgdt();
}
