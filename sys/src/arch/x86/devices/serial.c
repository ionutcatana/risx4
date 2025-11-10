#include <devices/serial.h>
#include <x86/io.h>

#include <stdbool.h>
#include <stdint.h>

static bool serial_initialized = false;

int initserial(void) {
    outs(X86_SERIAL_PORT + 1, 0x00);    // disable all interrupts
    outs(X86_SERIAL_PORT + 3, 0x80);    // enable dlab (set baud rate divisor)
    outs(X86_SERIAL_PORT + 0, 0x03);    // set divisor to 3 (lo byte) 38400 baud
    outs(X86_SERIAL_PORT + 1, 0x00);    //                  (hi byte)
    outs(X86_SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outs(X86_SERIAL_PORT + 2, 0xc7);    // enable fifo, clear them, 14-byte threshold
    outs(X86_SERIAL_PORT + 4, 0x0b);    // irqs enabled, rts/dsr set
    outs(X86_SERIAL_PORT + 4, 0x1e);    // set in the mcr

    // send test byte and check if serial is faulty
    outs(X86_SERIAL_PORT + 0, 0xae);
    if(ins(X86_SERIAL_PORT + 0) != 0xae) {
        return 1;
    }

    // if serial is not faulty set it in normal operation mode:
    // not-loopback with irqs enabled and out#1 and out#2 bits enabled
    outs(X86_SERIAL_PORT + 4, 0x0f);

    serial_initialized = true;
    return 0;
}

void serialputchar(int c) {
    if (!serial_initialized) {
        return;
    }

    while((ins(X86_SERIAL_PORT + 5) & 0x20) == 0);
    if(c == '\n') {
        outs(X86_SERIAL_PORT, (uint8_t)'\r');
        outs(X86_SERIAL_PORT, (uint8_t)'\n');
    } else {
        outs(X86_SERIAL_PORT, (uint8_t)c);
    }
}

void serialputs(const char* str) {
    // worth testing for performance?
    if (!serial_initialized) {
        return;
    }

    while(*str) {
        serialputchar((int)(*str));
        str++;
    }
}
