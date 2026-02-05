        .intel_syntax noprefix
        .section .text

        .global readcr0
readcr0:
        mov     rax, cr0
        ret

        .global readcr2
readcr2:
        mov     rax, cr2
        ret
        .global readcr3

readcr3:
        mov     rax, cr3
        ret

        .global readcr4
readcr4:
        mov     rax, cr4
        ret

        .global readcr8
readcr8:
        mov     rax, cr8
        ret

        .global readrflags
readrflags:
        pushfq
        pop     rax
        ret

        .global loadcr0
loadcr0:
        mov     cr0, rdi
        ret

        .global loadcr2
loadcr2:
        mov     cr2, rdi
        ret

        .global loadcr3
loadcr3:
        mov     cr3, rdi
        ret

        .global loadcr4
loadcr4:
        mov     cr4, rdi
        ret

        .global loadcr8
loadcr8:
        mov     cr8, rdi
        ret

        .global loadrflags
loadrflags:
        push    rdi
        popfq
        ret

        .global readlapicid
readlapicid:
        push    rbx
        mov     rax, 1
        cpuid
        mov     rax, rbx
        shr     rax, 24
        pop     rbx
        ret
