        .intel_syntax noprefix
        .section .text
        .global ins
        .global outs
ins:    push    ebp
        mov     ebp, esp
        mov     edx, [ebp+8]
        in      al, dx
        movzx   eax, al                 # zero-extend to 32 bits
        pop     ebp
        ret

outs:   push    ebp
        mov     ebp, esp
        mov     edx, [ebp+8]
        mov     al, [ebp+12]
        out     dx, al
        pop     ebp
        ret
