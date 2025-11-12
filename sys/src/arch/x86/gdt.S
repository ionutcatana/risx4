        .intel_syntax noprefix

        .equ    RISX_CODE_SEG, 0x08
        .equ    RISX_DATA_SEG, 0x10
        .equ    USER_CODE_SEG, 0x1b
        .equ    USER_DATA_SEG, 0x23
        .equ    TSS_SEGMENT, 0x28

        .section .text
        .global loadgdt
        .extern _desc
loadgdt:
        push    rsp
        mov     rbp, rsp

#       lgdt    0
        push    rax

#       lea     rax, [rip + 3]
#       push    rax
#       lretq

        cli
        lgdt    [_desc]                 # load global descriptor table

#       jmp     RISX_CODE_SEG:L1
        push    RISX_CODE_SEG           # 64-bit GAS requires this incantation
        lea     rax, [rip + L1]
        push    rax
        lretq                           # update code segment register
L1:     mov     ax, RISX_DATA_SEG       # update other segment registers
        mov     ds, ax
        mov     es, ax
        mov     fs, ax

        mov     ax, USER_DATA_SEG       # update user data segment registers
        mov     gs, ax
        mov     ss, ax

        pop     rax

        mov     rsp, rbp
        pop     rbp
        ret
