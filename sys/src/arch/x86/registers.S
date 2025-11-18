        .intel_syntax noprefix
        .section .text

        .global readcr0
        .global readcr2
        .global readcr3
        .global readcr4
        .global readcr8
readcr0:
        mov     rax, cr0
        ret
readcr2:
        mov     rax, cr2
        ret
readcr3:
        mov     rax, cr3
        ret
readcr4:
        mov     rax, cr4
        ret
readcr8:
        mov     rax, cr8
        ret

