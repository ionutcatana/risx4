        .intel_syntax noprefix
        .section .text
        .global _start
_start: push    0                       # 64K stack is provided
        popfq                           # clear RFLAGS register
#       mov     rdi, __bss_start        # clear bss
#       mov     rcx, __bss_size
#       xor     rax, rax
#       rep     stosb

        call    risx                    # should never return
        call    halt
