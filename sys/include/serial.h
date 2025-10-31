#ifndef SERIAL_H
#define SERIAL_H 1

#include <stdint.h>

#define SERIAL_PORT 0x3F8

// i686/serial.S
extern uint8_t ins(uint32_t port);
extern void outs(uint32_t port, uint8_t data);
// i686/serial.c
int initserial();

#endif
