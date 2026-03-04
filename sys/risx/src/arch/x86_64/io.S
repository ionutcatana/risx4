        .intel_syntax noprefix

        # # # # # # # # # # # # # # # #
        #       EXPORTED SYMBOLS      #
        # # # # # # # # # # # # # # # #
        .global reads
        .global writes
        .global readw
        .global writew
        .global readl
#       .global writeq
#       .global readq
        .global writel
        .global io_wait

        # # # # # # # # # # # # # # # #
        #       TEXT SECTION          #
        # # # # # # # # # # # # # # # #
        .code64
        .section .text
reads:  mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      al, dx                  # read byte from port dx into al
        ret

writes: mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, al                  # write byte from al to port dx
        ret

readw:  mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      ax, dx                  # read word from port dx into ax
        ret

writew: mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, ax                  # write word from ax to port dx
        ret

readl:  mov     edx, edi                # move port number to edx
        xor     eax, eax                # clear rax
        in      eax, dx                 # read dword from port dx into eax
        ret

writel: mov     edx, edi                # move port number to edx
        mov     eax, esi                # move data to eax
        out     dx, eax                 # write dword from eax to port dx
        ret

io_wait:
        mov     edx, 0x80               # waste some i/o time on purpose
        in      al, dx
        ret
