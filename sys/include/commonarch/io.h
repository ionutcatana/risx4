#ifndef IO_H
#define IO_H 1

#include <stdint.h>

#define X86_SERIAL_PORT (0x3f8)

// arch/x86/io.S
uint8_t     reads(uint32_t port);
void        writes(uint32_t port, uint8_t data);

uint16_t    readw(uint32_t port);
void        writew(uint32_t port, uint16_t data);

uint32_t    readl(uint32_t port);
void        writel(uint32_t port, uint32_t data);

uint64_t    readq(uint32_t port);
void        writeq(uint32_t port, uint64_t data);

#endif
