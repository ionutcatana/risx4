        .intel_syntax noprefix

        .equ    RISX_CODE_SEG, 0x08
        .equ    RISX_DATA_SEG, 0x10
        .equ    USER_DATA_SEG, 0x1b
        .equ    USER_CODE_SEG, 0x23
        .equ    RISX_TSS0_SEG, 0x28

        .global _gdt                    # uint64_t _gdt[69]
        .global _tssaddrs               # uint64_t _tssaddrs[32]
        .global loadgdt
        .global loadtr

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
_tss21: .space  104
        .align  16
_tss22: .space  104
        .align  16
_tss23: .space  104
        .align  16
_tss24: .space  104
        .align  16
_tss25: .space  104
        .align  16
_tss26: .space  104
        .align  16
_tss27: .space  104
        .align  16
_tss28: .space  104
        .align  16
_tss29: .space  104
        .align  16
_tss30: .space  104
        .align  16
_tss31: .space  104

        .section .data
        .align  16
_tssaddrs:                              # uint64_t _tssaddrs[32]
        .quad   _tss0
        .quad   _tss1
        .quad   _tss2
        .quad   _tss3
        .quad   _tss4
        .quad   _tss5
        .quad   _tss6
        .quad   _tss7
        .quad   _tss8
        .quad   _tss9
        .quad   _tss10
        .quad   _tss11
        .quad   _tss12
        .quad   _tss13
        .quad   _tss14
        .quad   _tss15
        .quad   _tss16
        .quad   _tss17
        .quad   _tss18
        .quad   _tss19
        .quad   _tss20
        .quad   _tss21
        .quad   _tss22
        .quad   _tss23
        .quad   _tss24
        .quad   _tss25
        .quad   _tss26
        .quad   _tss27
        .quad   _tss28
        .quad   _tss29
        .quad   _tss30
        .quad   _tss31

        .code64
        .section .text
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
