#ifndef CONFIG_H
#define CONFIG_H 1

/* risx release number and codename                                           */
#define VERSION             "0.0.7"
#define CODENAME            "Initial commit"

/* maximum number of entries in the process table for user-mode processes     */
#define NPROC               224
#define NTASK               32

/* stack size and max core count options. all stacks fit on a single 2mib mega
   page. possible configs: 32 cores+64k stacks or 128 cores+16k stacks        */
#define STACK_SIZE          65536
#define NCPU                32

/* kernel heap base address (hhdm+4tib)                                       */
#define KERNEL_HEAP_BASE    0xFFFFA00000000000

#endif
