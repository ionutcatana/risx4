OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

PHDRS
{
    limine_requests PT_LOAD;
    text            PT_LOAD;
    rodata          PT_LOAD;
    data            PT_LOAD;
}

SECTIONS
{
    /* risx is a higher-half kernel, it is running in virtual memory at the */
    /* last 2GB of the address space. */
    . = 0xffffffff80000000;

    .limine_requests :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .text : {
        /* . = ALIGN(16); */
        __text_start = .;
        KEEP(*(.text))
        KEEP(*(.text*))
        __text_end = .;
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .rodata : {
        /* . = ALIGN(16); */
        __rodata_start = .;
        KEEP(*(.rodata))
        KEEP(*(.rodata*))
        __rodata_end = .;
    } :rodata

    /* Add a .note.gnu.build-id output section in case a build ID flag is */
    /* added to the linker command. */
    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .data : {
        /* . = ALIGN(16); */
        __data_start = .;
        KEEP(*(.data))
        KEEP(*(.data*))
        KEEP(*(font.o))
    } :data

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .bss : {
        /* . = ALIGN(16); */
        /* __bss_start = .; */
        KEEP(*(.bss))
        KEEP(*(.bss*))
        KEEP(*(COMMON))
        /* __bss_end = .; */
        __data_end = .;
    } :data

    /* __bss_size = __bss_end - __bss_start; */

    /* these segments can cause issues */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
