        .section .text
        .global _start
_start: mv      a0, sp
        call    setup
        call    risx
        call    abort
