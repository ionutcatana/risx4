#ifndef COMMONARCH_IO_H
#define COMMONARCH_IO_H 1
// port i/o is characteristic of x86, but i decided to make this a "commonarch"
// header

#include <stdint.h>

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
