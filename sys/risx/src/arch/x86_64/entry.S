        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global _start_bootstrap
        .global _start_application
        .global _stacks
        .global _guards

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
_start_bootstrap:                       # entry point on bootstrap cpu
        call    setup                   # load new address space after setup
_start_application:                     # all cores execute this
#       lea     rax, kernelpagetbl
#       mov     cr3, rax
#       mov     rax, 0xffffffffffffe000
#       mov     rsp, rax
#       xor     rbp, rbp
        xor     rbp, rbp
        lea     rsp, _stack0            # use own stack
        call    risx                    # should never return
        call    abort

        # # # # # # # # # # # # # # # #
        #       DATA SECTION          #
        # # # # # # # # # # # # # # # #
        .section .data
        .align  16
_stacks:.quad   _stack0                 # uint64_t _stacks[32]
        .quad   _stack1
        .quad   _stack2
        .quad   _stack3
        .quad   _stack4
        .quad   _stack5
        .quad   _stack6
        .quad   _stack7
        .quad   _stack8
        .quad   _stack9
        .quad   _stack10
        .quad   _stack11
        .quad   _stack12
        .quad   _stack13
        .quad   _stack14
        .quad   _stack15
        .quad   _stack16
        .quad   _stack17
        .quad   _stack18
        .quad   _stack19
        .quad   _stack20
        .quad   _stack21
        .quad   _stack22
        .quad   _stack23
        .quad   _stack24
        .quad   _stack25
        .quad   _stack26
        .quad   _stack27
        .quad   _stack28
        .quad   _stack29
        .quad   _stack30
        .quad   _stack31

        .align  16
_guards:.quad   _guard0                 # uint64_t _guards[32]
        .quad   _guard1
        .quad   _guard2
        .quad   _guard3
        .quad   _guard4
        .quad   _guard5
        .quad   _guard6
        .quad   _guard7
        .quad   _guard8
        .quad   _guard9
        .quad   _guard10
        .quad   _guard11
        .quad   _guard12
        .quad   _guard13
        .quad   _guard14
        .quad   _guard15
        .quad   _guard16
        .quad   _guard17
        .quad   _guard18
        .quad   _guard19
        .quad   _guard20
        .quad   _guard21
        .quad   _guard22
        .quad   _guard23
        .quad   _guard24
        .quad   _guard25
        .quad   _guard26
        .quad   _guard27
        .quad   _guard28
        .quad   _guard29
        .quad   _guard30
        .quad   _guard31

        # # # # # # # # # # # # # # # #
        #       BSS SECTION           #
        # # # # # # # # # # # # # # # #
        .section .bss
        .align  0x1000
_stack0:                                # each stack is 16K in size
        .space  0x4000
        .align  0x1000
_guard0:                                # each stack is followed by a guard page
        .space  0x1000                  # to prevent overflowing
        .align  0x1000
_stack1:
        .space  0x4000
        .align  0x1000
_guard1:
        .space  0x1000
        .align  0x1000
_stack2:
        .space  0x4000
        .align  0x1000
_guard2:
        .space  0x1000
        .align  0x1000
_stack3:
        .space  0x4000
        .align  0x1000
_guard3:
        .space  0x1000
        .align  0x1000
_stack4:
        .space  0x4000
        .align  0x1000
_guard4:
        .space  0x1000
        .align  0x1000
_stack5:
        .space  0x4000
        .align  0x1000
_guard5:
        .space  0x1000
        .align  0x1000
_stack6:
        .space  0x4000
        .align  0x1000
_guard6:
        .space  0x1000
        .align  0x1000
_stack7:
        .space  0x4000
        .align  0x1000
_guard7:
        .space  0x1000
        .align  0x1000
_stack8:
        .space  0x4000
        .align  0x1000
_guard8:
        .space  0x1000
        .align  0x1000
_stack9:
        .space  0x4000
        .align  0x1000
_guard9:
        .space  0x1000
        .align  0x1000
_stack10:
        .space  0x4000
        .align  0x1000
_guard10:
        .space  0x1000
        .align  0x1000
_stack11:
        .space  0x4000
        .align  0x1000
_guard11:
        .space  0x1000
        .align  0x1000
_stack12:
        .space  0x4000
        .align  0x1000
_guard12:
        .space  0x1000
        .align  0x1000
_stack13:
        .space  0x4000
        .align  0x1000
_guard13:
        .space  0x1000
        .align  0x1000
_stack14:
        .space  0x4000
        .align  0x1000
_guard14:
        .space  0x1000
        .align  0x1000
_stack15:
        .space  0x4000
        .align  0x1000
_guard15:
        .space  0x1000
        .align  0x1000
_stack16:
        .space  0x4000
        .align  0x1000
_guard16:
        .space  0x1000
        .align  0x1000
_stack17:
        .space  0x4000
        .align  0x1000
_guard17:
        .space  0x1000
        .align  0x1000
_stack18:
        .space  0x4000
        .align  0x1000
_guard18:
        .space  0x1000
        .align  0x1000
_stack19:
        .space  0x4000
        .align  0x1000
_guard19:
        .space  0x1000
        .align  0x1000
_stack20:
        .space  0x4000
        .align  0x1000
_guard20:
        .space  0x1000
        .align  0x1000
_stack21:
        .space  0x4000
        .align  0x1000
_guard21:
        .space  0x1000
        .align  0x1000
_stack22:
        .space  0x4000
        .align  0x1000
_guard22:
        .space  0x1000
        .align  0x1000
_stack23:
        .space  0x4000
        .align  0x1000
_guard23:
        .space  0x1000
        .align  0x1000
_stack24:
        .space  0x4000
        .align  0x1000
_guard24:
        .space  0x1000
        .align  0x1000
_stack25:
        .space  0x4000
        .align  0x1000
_guard25:
        .space  0x1000
        .align  0x1000
_stack26:
        .space  0x4000
        .align  0x1000
_guard26:
        .space  0x1000
        .align  0x1000
_stack27:
        .space  0x4000
        .align  0x1000
_guard27:
        .space  0x1000
        .align  0x1000
_stack28:
        .space  0x4000
        .align  0x1000
_guard28:
        .space  0x1000
        .align  0x1000
_stack29:
        .space  0x4000
        .align  0x1000
_guard29:
        .space  0x1000
        .align  0x1000
_stack30:
        .space  0x4000
        .align  0x1000
_guard30:
        .space  0x1000
        .align  0x1000
_stack31:
        .space  0x4000
        .align  0x1000
_guard31:
        .space  0x1000
        .align  0x1000
