        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global _start_bootstrap
        .global _start_application

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
_start_bootstrap:                       # entry point on bootstrap cpu
        call    setup                   # load new address space after setup
#       lea     rax, kernelpagetbl
#       mov     cr3, rax
#       mov     rax, 0xffffffffffffe000
#       mov     rsp, rax
#       xor     rbp, rbp
_start_application:                     # all cores execute this
        call    risx                    # should never return
        call    abort

        # # # # # # # # # # # # # # # #
        #       DATA SECTION          #
        # # # # # # # # # # # # # # # #
        .section .data
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

        # # # # # # # # # # # # # # # #
        #       BSS SECTION           #
        # # # # # # # # # # # # # # # #
        .section .bss
        .align  16
_stack0:
        .space  16384
        .align  16
_stack1:
        .space  16384
        .align  16
_stack2:
        .space  16384
        .align  16
_stack3:
        .space  16384
        .align  16
_stack4:
        .space  16384
        .align  16
_stack5:
        .space  16384
        .align  16
_stack6:
        .space  16384
        .align  16
_stack7:
        .space  16384
        .align  16
_stack8:
        .space  16384
        .align  16
_stack9:
        .space  16384
        .align  16
_stack10:
        .space  16384
        .align  16
_stack11:
        .space  16384
        .align  16
_stack12:
        .space  16384
        .align  16
_stack13:
        .space  16384
        .align  16
_stack14:
        .space  16384
        .align  16
_stack15:
        .space  16384
        .align  16
_stack16:
        .space  16384
        .align  16
_stack17:
        .space  16384
        .align  16
_stack18:
        .space  16384
        .align  16
_stack19:
        .space  16384
        .align  16
_stack20:
        .space  16384
        .align  16
_stack21:
        .space  16384
        .align  16
_stack22:
        .space  16384
        .align  16
_stack23:
        .space  16384
        .align  16
_stack24:
        .space  16384
        .align  16
_stack25:
        .space  16384
        .align  16
_stack26:
        .space  16384
        .align  16
_stack27:
        .space  16384
        .align  16
_stack28:
        .space  16384
        .align  16
_stack29:
        .space  16384
        .align  16
_stack30:
        .space  16384
        .align  16
_stack31:
        .space  16384
        .align  16
