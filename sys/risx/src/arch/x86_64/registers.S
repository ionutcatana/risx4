        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global readcr0
        .global readcr2
        .global readcr3
        .global readcr4
        .global readcr8
        .global readrflags
        .global loadcr0
        .global loadcr2
        .global loadcr3
        .global loadcr4
        .global loadcr8
        .global loadrflags
        .global readlapicid

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
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

readrflags:
        pushfq
        pop     rax
        ret

loadcr0:
        mov     cr0, rdi
        ret

loadcr2:
        mov     cr2, rdi
        ret

loadcr3:
        mov     cr3, rdi
        ret

loadcr4:
        mov     cr4, rdi
        ret

loadcr8:
        mov     cr8, rdi
        ret

loadrflags:
        push    rdi
        popfq
        ret

readlapicid:
        push    rbx
        mov     rax, 1
        cpuid
        mov     rax, rbx
        shr     rax, 24
        pop     rbx
        ret
