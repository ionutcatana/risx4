        .intel_syntax noprefix

        .global _start

        .code64
        .section .text
_start: mov     rsp, 0x00007ffffffff000
        xor     rbp, rbp
        mov     rdi, 1
        lea     rsi, _argv0

        call    main                    # no syscall implemented yet

        .section .rodata
        .align  16
dummy:  .asciz  "dummy"
        .align  16
_argv0: .long   dummy
