        .intel_syntax noprefix

        .equ    MAGIC, 0xE85250D6
#if defined(__i386__)
        .equ    ARCH, 0                 # 32-bit protected mode x86
#elif defined(__mips__)
        .equ    ARCH, 4                 # 32-bit MIPS
#else
#error "Unsupported architecture"
#endif
        .equ    LENGTH, (mbend - mbhead)
        .equ    CHECKSUM, (-(MAGIC + ARCH + (mbend - mbhead)))

        .equ    INFO, 1                 # request information
        .equ    REQ_MMAP, 6             # memory map
        .equ    REQ_CMDLINE, 1          # command line args
        .equ    REQ_FRAMEBUFFER, 8      # framebuffer information

        .equ    FRAMEBUFFER, 5          # framebuffer preferences

        .section .multiboot2
        .global multiboot2_header

        .align  8                       # aligned to 8 bytes
mbhead: .long   MAGIC                   # multiboot 2 header
        .long   ARCH
        .long   LENGTH
        .long   CHECKSUM
        .align  8                       # every tag needs to be aligned
mbinfo: .word   INFO
        .word   0                       # required
        .long   20
        .long   REQ_MMAP
        .long   REQ_CMDLINE
        .long   REQ_FRAMEBUFFER
        .align  8
mbfb:   .word   FRAMEBUFFER
        .word   0                       # required
        .long   20
        .long   1024                    # width
        .long   768                     # height
        .long   24                      # depth
        .align  8
mbfoot: .word   0                       # last required tag
        .word   0
        .long   8
mbend:
