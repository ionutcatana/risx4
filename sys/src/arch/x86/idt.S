        .intel_syntax noprefix
        .section .text
        .global loadidt
loaditd:
        lidt    [_idtr]
        ret
