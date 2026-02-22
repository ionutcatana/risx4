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

        .global inw
inw:    mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      ax, dx                  # read word from port dx into ax
        ret

        .global outw
outw:   mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, ax                  # write word from ax to port dx
        ret

        .global inl
inl:    mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      eax, dx                 # read dword from port dx into eax
        ret

        .global outl
outl:   mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, eax                 # write dword from eax to port dx
        ret

io_wait:
        mov     edx, 0x80               # waste some i/o time on purpose
        in      al, dx
        ret
