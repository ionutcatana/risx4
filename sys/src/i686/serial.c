#include "serial.h"
#include <stdint.h>

int initserial() {
    outs(SERIAL_PORT + 1, 0x00);    // disable all interrupts
    outs(SERIAL_PORT + 3, 0x80);    // enable dlab (set baud rate divisor)
    outs(SERIAL_PORT + 0, 0x03);    // set divisor to 3 (lo byte) 38400 baud
    outs(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
    outs(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outs(SERIAL_PORT + 2, 0xC7);    // enable fifo, clear them, 14-byte threshold
    outs(SERIAL_PORT + 4, 0x0B);    // irqs enabled, rts/dsr set
    outs(SERIAL_PORT + 4, 0x1E);    // set in the mcr

    // send test byte and check if serial is faulty
    outs(SERIAL_PORT + 0, 0xAE);
    if(ins(SERIAL_PORT + 0) != 0xAE) {
        return 1;
    }

    // if serial is not faulty set it in normal operation mode:
    // not-loopback with irqs enabled and out#1 and out#2 bits enabled
    outs(SERIAL_PORT + 4, 0x0F);
    return 0;
}
