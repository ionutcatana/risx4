OUTPUT_FORMAT(elf64-littleaarch64)
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

    .limine_requests :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

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

    /* add a .note.gnu.build-id output section in case a build id flag is   */
    /* added to the linker command.                                         */
    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .data : {
        KEEP(*(.data))
        KEEP(*(.data*))
        KEEP(*(font.o))
    } :data

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    .bss : {
        KEEP(*(.bss))
        KEEP(*(.bss*))
        KEEP(*(COMMON))
    } :data

    /* these segments can cause issues                                       */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
