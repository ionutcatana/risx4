        .intel_syntax noprefix
        .section .text
        .global exchange
exchange:
        xchg    [rdi], rsi
        mov     rax, rsi
        ret
