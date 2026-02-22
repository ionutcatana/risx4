        .intel_syntax noprefix
        .section .text

        .global intenable
        .global intdisable

intenable:
        sti
        ret

intdisable:
        cli
        ret
