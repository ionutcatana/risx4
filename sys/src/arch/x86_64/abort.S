        .intel_syntax noprefix
        .section .text
        .global abort
abort:  cli
.L1:    hlt
        jmp     .L1
