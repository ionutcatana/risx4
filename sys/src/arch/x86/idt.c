#include <x86/gdt.h>
#include <x86/idt.h>

#include <stdint.h>
#include <stddef.h>

static union idt_descriptor idt[IDT_SIZE];
struct idtr _idtr; // used in idt.S

void loadidt(void);
void initidt(void) {
    _idtr.limit = sizeof(idt) - 1;
    _idtr.base = (uintptr_t)&idt;
}

void sethandler(size_t index, uintptr_t handler,
                uint8_t ist, uint8_t attributes) {
    idt[index].base_lower = (uint16_t)(handler & 0xFFFF);
    idt[index].selector = RISX_CODE_SEG;
    idt[index].ist = ist;
    idt[index].attributes = attributes;
    idt[index].base_middle = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[index].base_upper = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    idt[index].reserved = 0;
}

