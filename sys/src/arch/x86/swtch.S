        .intel_syntax noprefix
        .section .text
        .global swtch

        # void swtch(struct context **old, struct context *new);
        # Save current registers in old (if old != NULL), load from new.
swtch:  # Save old callee-save registers
        push    rbp
        push    rbx
        push    r12
        push    r13
        push    r14
        push    r15
                                        # Switch stacks
        mov     [rdi], rsp              # *old = rsp
        mov     rsp, rsi                # rsp = new

        pop     r15                     # Load new callee-save registers
        pop     r14
        pop     r13
        pop     r12
        pop     rbx
        pop     rbp

        ret

        .global kthread_entry
        .extern exit
        .extern sched_release_lock

kthread_entry:
        # Arrive here from swtch with interrupts DISABLED
        # RBX contains the function pointer (restored by swtch)
        # We hold sched_lock (acquired by scheduler before switch)

        call    sched_release_lock
        sti                 # Enable interrupts
        call    rbx         # Run thread function
        call    exit        # Handle return

.global enter_usermode
enter_usermode:
        # R12 holds the pointer to trapframe (passed via context restoration)
        mov     rsp, r12

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

        add     rsp, 16         # Skip vector and error code

        swapgs
        iretq
