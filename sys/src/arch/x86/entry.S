        .intel_syntax noprefix
        .section .text
        .global _start
_start: mov     rdi, rsp
        call    risx                    # should never return
        call    abort
