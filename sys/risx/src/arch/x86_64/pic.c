#include "arch/x86_64/specific/pic.h"
#include "commonarch/io.h"
#include <stdint.h>

/* i8259 PIC ports                                                            */
#define PIC1_CMD    0x20
#define PIC1_DATA   0x21
#define PIC2_CMD    0xA0
#define PIC2_DATA   0xA1

void
disablepic(void)
{
    /* icw1: begin initialization sequence */
    writes(PIC1_CMD, 0x11);
    writes(PIC2_CMD, 0x11);

    /* icw2: remap IRQs to vectors 0x20-0x2F */
    writes(PIC1_DATA, 0x20);
    writes(PIC2_DATA, 0x28);

    /* icw3: cascading */
    writes(PIC1_DATA, 0x04);
    writes(PIC2_DATA, 0x02);

    /* icw4: 8086 mode */
    writes(PIC1_DATA, 0x01);
    writes(PIC2_DATA, 0x01);

    /* mask all IRQs */
    writes(PIC1_DATA, 0xFF);
    writes(PIC2_DATA, 0xFF);
}
