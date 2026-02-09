        .intel_syntax noprefix
        .section .text

        .global istub
        .extern idispatch

#       push    ss                      # happens automatically
#       push    rsp
#       push    rflags
#       push    cs
#       push    rip
istub:  push    rax                     # save the general purpose registers
        push    rbx
        push    rcx
        push    rdx
        push    rsi
        push    rdi
#       push    rsp
        push    rbp
        push    r8
        push    r9
        push    r10
        push    r11
        push    r12
        push    r13
        push    r14
        push    r15

        # Check for user mode (CS & 3 == 3)
        # RSP points to saved R15.
        # Registers: 15 * 8 = 120 bytes.
        # Stack: [R15 ... RAX] [VEC] [ERR] [RIP] [CS]
        # Offset to CS: 120 + 8 (VEC) + 8 (ERR) + 8 (RIP) = 144 = 18 * 8.
        mov     rax, [rsp + 144]
        and     rax, 3
        cmp     rax, 3
        jne     1f
        swapgs
1:

        mov     rdi, rsp                # move trapframe into first argument
        call    idispatch

        # Restore GS if returning to user mode
        mov     rax, [rsp + 144]
        and     rax, 3
        cmp     rax, 3
        jne     2f
        swapgs
2:

        pop     r15
        pop     r14

        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     r11
        pop     r10
        pop     r9
        pop     r8
        pop     rbp
#       pop     rsp
        pop     rdi
        pop     rsi
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

        add     rsp, 16                 # remove the vector number & error code

        iretq
#       pop     rip                     # happens automatically
#       pop     cs
#       pop     rflags
#       pop     rsp
#       pop     ss

