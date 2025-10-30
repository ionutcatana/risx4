ENTRY(_start)

SECTIONS
{
    . = 2M;

    .text : {
        . = ALIGN(16);
        KEEP(*(.multiboot2))
        KEEP(*(.text))
    }

    .rodata : {
        . = ALIGN(16);
        KEEP(*(.rodata))
    }

    .data : {
        . = ALIGN(16);
        KEEP(*(.data))
        . = ALIGN(16);
        __font_start = .;
        KEEP(*(font.o))
    }

    __bss_start = .;
    .bss : {
        . = ALIGN(16);
        KEEP(*(.bss))
        KEEP(*(COMMON))
    }
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
}
