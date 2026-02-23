        .intel_syntax noprefix

        .equ    RISX_CODE_SEG, 0x08
        .equ    RISX_DATA_SEG, 0x10
        .equ    USER_DATA_SEG, 0x1b
        .equ    USER_CODE_SEG, 0x23
        .equ    RISX_TSS0_SEG, 0x28

        .global _gdt                    # uint64_t _gdt[69]
#       .global _desc
        .global _tss0                   # uint64_t _tss0[13]
        .global _tss1
        .global _tss2
        .global _tss3
        .global _tss4
        .global _tss5
        .global _tss6
        .global _tss7
        .global _tss8
        .global _tss9
        .global _tss10
        .global _tss11
        .global _tss12
        .global _tss13
        .global _tss14
        .global _tss15
        .global _tss16
        .global _tss17
        .global _tss18
        .global _tss19
        .global _tss20
        .global _tss31
        .global _tss32
        .global _tss33
        .global _tss34
        .global _tss35
        .global _tss36
        .global _tss37
        .global _tss38
        .global _tss39
        .global _tss30
        .global _tss31
        .global _tss32

        .section .data
        .align  16
_gdt:   .quad   0                       # null descriptor
        .quad   0x00A09A000000FFFF      # risx code seg
        .quad   0x00C092000000FFFF      # risx data seg
        .quad   0x00C0F2000000FFFF      # user code seg
        .quad   0x00A0FA000000FFFF      # user data seg
        .quad   0                       # 32 empty tss descriptors
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
        .quad   0
_desc:  .word   (_desc - _gdt - 1)      # limit
        .quad   _gdt                    # base

        .section .bss
        .align  16
        .extern _tss0
_tss0:  .space  104                     # 32 task state segments
        .align  16
_tss1:  .space  104
        .align  16
_tss2:  .space  104
        .align  16
_tss3:  .space  104
        .align  16
_tss4:  .space  104
        .align  16
_tss5:  .space  104
        .align  16
_tss6:  .space  104
        .align  16
_tss7:  .space  104
        .align  16
_tss8:  .space  104
        .align  16
_tss9:  .space  104
        .align  16
_tss10: .space  104
        .align  16
_tss11: .space  104
        .align  16
_tss12: .space  104
        .align  16
_tss13: .space  104
        .align  16
_tss14: .space  104
        .align  16
_tss15: .space  104
        .align  16
_tss16: .space  104
        .align  16
_tss17: .space  104
        .align  16
_tss18: .space  104
        .align  16
_tss19: .space  104
        .align  16
_tss20: .space  104
        .align  16
_tss31: .space  104
        .align  16
_tss32: .space  104
        .align  16
_tss33: .space  104
        .align  16
_tss34: .space  104
        .align  16
_tss35: .space  104
        .align  16
_tss36: .space  104
        .align  16
_tss37: .space  104
        .align  16
_tss38: .space  104
        .align  16
_tss39: .space  104
        .align  16
_tss30: .space  104
        .align  16
_tss31: .space  104
        .align  16
_tss32: .space  104

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
