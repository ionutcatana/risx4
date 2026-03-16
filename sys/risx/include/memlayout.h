#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H 1

#include <stdint.h>

/* stacks are made of 4 pages, preceeded by a guard page                      */
#define DISTANCE      ((uint64_t) 0x5000)             // 20K
#define STACK_SIZE    ((uint64_t) 0x4000)             // 16K

/* TODO: future idea. the stacks work fine right now                          */
/* 32 kernel stacks                                                           */
#define STACKKBASE0   ((uint64_t) 0xfffffffffffff000) // -4K
#define STACKK0       ((uint64_t) 0xffffffffffffe000) // -8K

/* 32 interrupt stacks                                                        */
#define STACKIBASE0   ((uint64_t) 0x00007ffffffff000) //
#define STACKI0       ((uint64_t) 0x00007fffffffe000) //

/* this is the first canonical virtual address of the higher half             */
#define HHDMBASE      ((uint64_t) 0xffff800000000000) // -512GB

/* virtual address of user process stack                                      */
#define STACKUBASE0   ((uint64_t) 0x00007ffffffff000)
#define STACKU0       ((uint64_t) 0x00007fffffffe000)

#endif
