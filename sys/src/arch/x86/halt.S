        .intel_syntax noprefix
        .section .text
        .global halt
halt:   cli
.L1:    hlt
        jmp     .L1
