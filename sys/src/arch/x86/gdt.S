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
#       push    rsp
#       mov     rbp, rsp

        cli
        lgdt    [_desc]                 # load global descriptor table

        mov     ax, RISX_DATA_SEG       # update other segment registers
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
#       mov     ax, USER_DATA_SEG       # update user data segment registers
#       mov     gs, ax
#       mov     ss, ax

        call    L1
L1:     pop     rdi
        push    RISX_CODE_SEG           # 64-bit GAS requires this incantation
        push    rdi
        lretq                           # update code segment register

#       mov     rsp, rbp
#       pop     rbp
        ret
