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
    /* last 2GB of the address space.                                       */
    . = 0xffffffff80000000;
    __kernel_start = .;

    .limine_requests :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    __text_start = .;
    .text : {
        KEEP(*(.text))
        KEEP(*(.text*))
    } :text
    __text_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    __rodata_start = .;
    .rodata : {
        KEEP(*(.rodata))
        KEEP(*(.rodata*))
    } :rodata

    /* add a .note.gnu.build-id output section in case a build id flag is   */
    /* added to the linker command.                                         */
    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata
    __rodata_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    __data_start = .;
    .data : {
        KEEP(*(.data))
        KEEP(*(.data*))
    } :data
    __data_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    __bss_start = .;
    .bss : {
        KEEP(*(.bss))
        KEEP(*(.bss*))
        KEEP(*(COMMON))
    } :data
    __bss_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    __kernel_end = .;
    __kernel_size = __kernel_end - __kernel_start;

    /* these segments can cause issues                                       */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
