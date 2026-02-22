        .intel_syntax noprefix
        .section .text

        .global enableinterrupts
enableinterrupts:
        sti
        ret

        .global disableinterrupts
disableinterrupts:
        cli
        ret
