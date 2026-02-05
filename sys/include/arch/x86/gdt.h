#ifndef X86_GDT_H
#define X86_GDT_H 1

#include <stdint.h>

#define GDT_SIZE        (7)

#define RISX_CODE_SEG   ((uint16_t)0x08)
#define RISX_DATA_SEG   ((uint16_t)0x10)
#define USER_DATA_SEG   ((uint16_t)0x1b)
#define USER_CODE_SEG   ((uint16_t)0x23)
#define TSS_SEG         ((uint16_t)0x28)

typedef struct {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed)) gdtr_t;

typedef union {
    struct {
        uint16_t limit;
        uint16_t base_lower;
        uint8_t base_middle;
        uint8_t access;
        uint8_t granularity;
        uint8_t base_upper;
    } __attribute__((packed));
    uint64_t value;
} gdtsegdesc_t;

typedef struct {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed)) tss_t;


// arch/x86/gdt.S
void    loadgdt(gdtr_t* desc);
void    loadtr(uint16_t selector);
// arch/x86/gdt.c
void    initgdt(void);
void    inittss(uint64_t id);

#endif
