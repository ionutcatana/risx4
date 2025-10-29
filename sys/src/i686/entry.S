        .intel_syntax noprefix

        .equ    STACK_SIZE, 0x1000      # 64K stack
        .equ    CODE_SEG_OFFSET, 0x08
        .equ    DATA_SEG_OFFSET, 0x10

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .section .text
        .global _start
_start: lea     esp, stack0             # initialize stack
        mov     ebp, esp
        push    0
        popf                            # clear EFLAGS register
        push    ebx                     # phys address of multiboot info struct
        push    eax                     # magic number

        cli
        lgdt    [_desc]                 # load global descriptor table

        jmp     CODE_SEG_OFFSET:L1      # update code segment register
L1:     mov     ax, DATA_SEG_OFFSET     # update other segment registers
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        mov     edi, __bss_start        # clear bss
        mov     ecx, __bss_size
        xor     ax, ax
        rep     stosb

        call    risx                    # should never return
        cli

halt:   jmp     halt

        # # # # # # # # # # # # # # # #
        #       BSS SECTION           #
        # # # # # # # # # # # # # # # #
        .section .bss
        .align  16
_top:   .skip   STACK_SIZE
stack0: .skip   STACK_SIZE              # stack for core 0
stack1:                                 # stack for core 1
