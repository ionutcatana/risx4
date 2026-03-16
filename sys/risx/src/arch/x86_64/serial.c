#include "arch/x86_64/io.h"
#include "arch/x86_64/serial.h"
#include "commonarch/io.h"
#include "commonarch/serial.h"
#include "sync/spinlock.h"
#include <stdbool.h>
#include <stdint.h>

static struct spinlock seriallk;

static bool initialized = false;

int initserial(void)
{
    initlock(&seriallk, "serial");

    writes(X86_64_SERIAL_PORT + 1, 0x00);    // disable all interrupts
    writes(X86_64_SERIAL_PORT + 3, 0x80);    // enable dlab (set baud rate divisor)
    writes(X86_64_SERIAL_PORT + 0, 0x03);    // set divisor to 3 (lo byte) 38400 baud
    writes(X86_64_SERIAL_PORT + 1, 0x00);    //                  (hi byte)
    writes(X86_64_SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    writes(X86_64_SERIAL_PORT + 2, 0xc7);    // enable fifo, clear them, 14-byte threshold
    writes(X86_64_SERIAL_PORT + 4, 0x0b);    // irqs enabled, rts/dsr set
    writes(X86_64_SERIAL_PORT + 4, 0x1e);    // set in the mcr

    // send test byte and check if serial is faulty
    writes(X86_64_SERIAL_PORT + 0, 0xae);
    if(reads(X86_64_SERIAL_PORT + 0) != 0xae)
        return 1;

    // if serial is not faulty set it in normal operation mode:
    // not-loopback with irqs enabled and out#1 and out#2 bits enabled
    writes(X86_64_SERIAL_PORT + 4, 0x0f);

    initialized = true;
    return 0;
}

void serialputchar(int c)
{
    acquire(&seriallk);
    if (initialized) {
        while((reads(X86_64_SERIAL_PORT + 5) & 0x20) == 0);
        if(c == '\n') {
            writes(X86_64_SERIAL_PORT, (uint8_t)'\r');
            writes(X86_64_SERIAL_PORT, (uint8_t)'\n');
        } else {
            writes(X86_64_SERIAL_PORT, (uint8_t)c);
        }
    }
    release(&seriallk);
}

