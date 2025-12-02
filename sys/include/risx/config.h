#ifndef CONFIG_H
#define CONFIG_H

/* risx release number and codename                                           */
#define RISXVERSION     "0.0.5"
#define RISXCODENAME    "Initial commit"

/* maximum number of entries in the process table for user-mode processes     */
#define PT_SIZE         128

/* physical page frame implementation                                         */
#define PMM_BITMAP
#define PMM_CURRENT     (PMM_BITMAP)
#define PMM_DEFAULT     (PMM_BITMAP)

#endif
