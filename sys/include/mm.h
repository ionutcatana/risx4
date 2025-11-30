#ifndef MM_H
#define MM_H 1

#define PAGESIZE    (0x1000)

// mm.c
void    initmm(void);
void    initpmm(void);
void*   allocframe(void);
void    freeframe(void* frame);
#endif
