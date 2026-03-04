        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global abort

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
abort:  cli
.L1:    hlt
        jmp     .L1
