#ifndef RISX_CONFIG_H
#define RISX_CONFIG_H 1

/* risx release number and codename                                           */
#define VERSION             "0.0.7"
#define CODENAME            "initial commit"

/* maximum number of entries in the process table for user-mode processes     */
#define NPROC               224
#define NTASK               32

/* maximum number of limine boot modules                                      */
#define MAX_MODULES         16

/* risx uses 32 cores and 16kib kernel stacks                                 */
#define NCPU                32

#endif
