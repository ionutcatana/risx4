OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

PHDRS
{
    text    PT_LOAD FLAGS (PF_R + PF_X);
    rodata  PT_LOAD FLAGS (PF_R);
    data    PT_LOAD FLAGS (PF_R + PF_W + PF_X);
}

SECTIONS
{
    . = 2M;

    .text : {
        /* . = ALIGN(16); */
        __text_start = .;
        KEEP(*(.multiboot2))
        KEEP(*(.text))
        __text_end = .;
    } : text

    .rodata : {
        /* . = ALIGN(16); */
        __rodata_start = .;
        KEEP(*(.rodata))
        __rodata_end = .;
    } : rodata

    .data : {
        /* . = ALIGN(16); */
        __data_start = .;
        KEEP(*(.data))
        KEEP(*(font.o))
    } : data

    .bss : {
        /* . = ALIGN(16); */
        __bss_start = .;
        KEEP(*(COMMON))
        KEEP(*(.bss))
        __bss_end = .;
        __data_end = .;
    }
    __bss_size = __bss_end - __bss_start;
}
