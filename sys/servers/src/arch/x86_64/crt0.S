        .intel_syntax noprefix

        .global _start
        .extern main

        .code64
        .section .text
_start: xor     rbp, rbp                # entry point known to the linker
        xor     rax, rax                # per ABI and compatibility with icc
run:    call    main
        jmp     run                     # no exit syscall yet, so just loop here

