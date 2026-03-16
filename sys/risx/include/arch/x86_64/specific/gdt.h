#ifndef X86_64_SPECIFIC_GDT_H
#define X86_64_SPECIFIC_GDT_H 1

#include "risx/config.h"
#include <stdint.h>

#define RISX_CODE_SEG   ((uint16_t)0x08)
#define RISX_DATA_SEG   ((uint16_t)0x10)
#define USER_DATA_SEG   ((uint16_t)0x1b)
#define USER_CODE_SEG   ((uint16_t)0x23)
#define RISX_TSS0_SEG   ((uint16_t)0x28)

#define NENTRIES_GDT    (5 + (NCPU * 2))

struct gdtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

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
} segdesc_t;

struct tss {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed));

// arch/x86_64/gdt.S
void loadgdt(struct gdtr* desc);
void loadtr(uint16_t selector);

// arch/x86_64/gdt.c
void initgdt(void);
void inittssdescriptors(void);
void inittss(void);

#endif
