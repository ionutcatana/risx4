        .section .text
        .global _start
_start: mv      a0, sp
        call    risx
        call    abort
