#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H 1

#include <stdint.h>

#define STACKBASE0    ((uint64_t) 0xffffffffffffefff) // -4K - 1
#define STACK0        ((uint64_t) 0xffffffffffffe000) // -8K
#define STACKBASEINT0 ((uint64_t) 0xffffffffff64efff) // -1276 - 1
#define STACKINT0     ((uint64_t) 0xffffffffff64b000) // -1280K
#define DISTANCE      ((uint64_t) 0x5000)             // 20K

#endif
