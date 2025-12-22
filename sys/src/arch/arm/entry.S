        .section .text
        .global _start
_start: mov     x0, sp
        bl      risx
        bl      abort
