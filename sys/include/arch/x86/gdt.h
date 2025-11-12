#ifndef X86_GDT_H
#define X86_GDT_H 1

#include <stdint.h>

#define RISX_CODE_SEG   (uint16_t)0x08
#define RISX_DATA_SEG   (uint16_t)0x10
#define USER_CODE_SEG   (uint16_t)0x1b
#define USER_DATA_SEG   (uint16_t)0x23
#define TSS_SEGMENT     (uint16_t)0x28

struct gdtr {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed));

union gdt_segment {
    struct {
        uint16_t limit;
        uint16_t base_lower;
        uint8_t base_middle;
        uint8_t access;
        uint8_t granularity;
        uint8_t base_upper;
    } __attribute__((packed));
    uint64_t value;
};

#define GDT_SIZE 7

// arch/x86/gdt.S
void loadgdt(void);
// arch/x86/gdt.c
void initgdt(void);

#endif
