        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global enterrisx

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
enterrisx:
        mov     rsp, rdi
        xor     rbp, rbp
        call    risx                    # should never return
        call    abort
