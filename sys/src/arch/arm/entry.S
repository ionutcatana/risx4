        .section .text
        .global _start
_start: mov     x0, sp
        bl      setup
        bl      risx
        bl      abort
