        .intel_syntax noprefix

        .global syscall_entry
        .extern syscall_handler

syscall_entry:
        swapgs
        mov     gs:[0], rsp
        mov     rsp, gs:[8]

        test    rsp, rsp
        jz      .Lno_thread

        mov     rsp, [rsp + 16]
        add     rsp, 16384

        push    0x1B
        push    qword ptr gs:[0]
        push    r11
        push    0x23
        push    rcx
        push    0
        push    0

        push    rax
        push    rbx
        push    rcx
        push    rdx
        push    rsi
        push    rdi
        push    rbp
        push    r8
        push    r9
        push    r10
        push    r11
        push    r12
        push    r13
        push    r14
        push    r15

        mov     rdi, rsp
        call    syscall_handler

        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     r11
        pop     r10
        pop     r9
        pop     r8
        pop     rbp
        pop     rdi
        pop     rsi
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

        add     rsp, 16

        pop     rcx
        add     rsp, 8
        pop     r11
        pop     rsp

        swapgs
        sysretq

.Lno_thread:
        swapgs
        sysretq
