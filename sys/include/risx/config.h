#ifndef CONFIG_H
#define CONFIG_H

/* risx release number and codename                                           */
#define RISX_VERSION    "0.0.6"
#define RISX_CODENAME   "Initial commit"

/* maximum number of entries in the process table for user-mode processes     */
#define PT_SIZE         128

/* physical page frame implementation                                         */
#define PMM_BITMAP
#define PMM_STACK
#define PMM_BUDDY
#define PMM_CURRENT     (PMM_BITMAP)
#define PMM_DEFAULT     (PMM_BITMAP)

/* stack size and max core count options. all stacks fit on a single 2mib mega
   page. possible configs: 32 cores+64k stacks or 128 cores+16k stacks        */
#define STACK_SIZE      65536
#define MAX_CPU_COUNT   32

#endif
