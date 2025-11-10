        .intel_syntax noprefix
        .section .text
        .global ins
ins:    mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      al, dx                  # read byte from port dx into al
        ret

        .global outs
outs:   mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, al                  # write byte from al to port dx
        ret
