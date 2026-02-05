        .intel_syntax noprefix
        .section .text
        .global enterrisx
enterrisx:
        mov     rsp, rdi
        xor     rbp, rbp
        call    risx                    # should never return
        call    abort
