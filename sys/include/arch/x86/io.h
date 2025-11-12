#ifndef X86_IO_H
#define X86_IO_H 1

#include <stdint.h>

#define X86_SERIAL_PORT 0x3f8

// arch/x86/io.S
extern uint8_t ins(uint32_t port);
extern void outs(uint32_t port, uint8_t data);


#endif
