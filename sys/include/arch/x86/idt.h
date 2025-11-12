#ifndef X86_IDT_H
#define X86_IDT_H 1

#include <stddef.h>
#include <stdint.h>

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

union idt_descriptor {
    struct {
        uint16_t base_lower;
        uint16_t selector;
        uint8_t ist;
        uint8_t attributes;
        uint16_t base_middle;
        uint32_t base_upper;
        uint32_t reserved;
    };
    uint64_t value;
} __attribute__((packed));

#define IDT_SIZE 256

// arch/x86/idt.S
void loadidt(void);
// arch/x86/idt.c
void initidt(void);
void sethandler(size_t index, uintptr_t handler,
                uint8_t ist, uint8_t attributes);

#endif
