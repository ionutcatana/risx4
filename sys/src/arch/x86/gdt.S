        .intel_syntax noprefix

        .equ    RISX_CODE_SEG, 0x08
        .equ    RISX_DATA_SEG, 0x10
        .equ    USER_DATA_SEG, 0x1b
        .equ    USER_CODE_SEG, 0x23
        .equ    TSS_SEG, 0x28

        .section .data
        .align  16
_gdt:   .quad   0                       # null descriptor
        .quad   0x00A09A000000FFFF      # risx code seg
        .quad   0x00C092000000FFFF      # risx data seg
        .quad   0x00C0F2000000FFFF      # user code seg
        .quad   0x00A0FA000000FFFF      # user data seg
        .quad   0                       # empty tss descriptor
        .quad   0
_desc:  .word   (_desc - _gdt - 1)      # limit
        .quad   _gdt                    # base

        .section .bss
        .align  16
_tss:   .space  104                     # task state segment

        .section .text
        .global loadgdt
        .global loadtr
loadgdt:
#       push    rsp
#       mov     rbp, rsp

        cli
        lgdt    [rdi]                   # load global descriptor table

        mov     ax, RISX_DATA_SEG       # update other segment registers
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
#       mov     ax, USER_DATA_SEG       # update user data segment registers

        call    L1
L1:     pop     rdi
        push    RISX_CODE_SEG           # gnu as requires this incantation
        push    rdi
        lretq                           # update code segment register

#       mov     rsp, rbp
#       pop     rbp
        ret

loadtr:
        ltr     di
        ret
