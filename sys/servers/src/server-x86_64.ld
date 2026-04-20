OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

PHDRS
{
    text   PT_LOAD;
    rodata PT_LOAD;
    data   PT_LOAD;
}

SECTIONS
{
    . = 0x0000000000400000;

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .text : {
        KEEP(*(.text))
        KEEP(*(.text*))
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .rodata : {
        KEEP(*(.rodata))
        KEEP(*(.rodata*))
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .data : {
        KEEP(*(.data))
        KEEP(*(.data*))
    } :data

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .bss : {
        KEEP(*(.bss))
        KEEP(*(.bss*))
        KEEP(*(COMMON))
    } :data

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
