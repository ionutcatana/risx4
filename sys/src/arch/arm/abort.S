        .section .text
        .global abort
abort:
.L1:    wfi
        bl      .L1

