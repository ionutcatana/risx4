        .intel_syntax noprefix
        .section .text
        .global _start
_start: push    0                       # 64K stack is provided
        popf                            # clear EFLAGS register
        mov     rdi, __bss_start        # clear bss
        mov     rcx, __bss_size
        xor     rax, rax
        rep     stosb

        call    setup                   # should never return
        cli

halt:   jmp     halt
