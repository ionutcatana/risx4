        .intel_syntax noprefix
        .section .text
        .global loadidt
loadidt:
        lidt    [_idtr]
        ret
