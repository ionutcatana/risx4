        .section .text
        .global abort
abort:
.L1:    wfi
        jmp     .L1
