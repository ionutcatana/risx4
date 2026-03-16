#include <stdio.h>
#include <stdint.h>

/* stacks are made of 4 pages, preceeded by a guard page                      */
#define DISTANCE      ((uint64_t) 0x5000)             // 20K
#define STACK_SIZE    ((uint64_t) 0x4000)             // 16K

/* 32 kernel stacks                                                           */
#define STACKKBASE0   ((uint64_t) 0xfffffffffffff000) // -4K
#define STACKK0       ((uint64_t) 0xffffffffffffe000) // -8K

/* 32 interrupt stacks                                                        */
#define STACKIBASE0   ((uint64_t) 0x00007ffffffff000) //
#define STACKI0       ((uint64_t) 0x00007fffffffe000) //

#define NCPU          (32)

int main() {
    uint64_t current_stack = STACKK0;
    uint64_t current_stackbase = STACKKBASE0;

    printf("\nKERNEL STACKS\n");
    printf("NCPU STACK              STACKBASE\n");
    for (size_t i = 0; i < NCPU; i++) {
        current_stack -= i * DISTANCE;
        current_stackbase -= i * DISTANCE;
        printf("%-4d 0x%16lx 0x%16lx\n",
            i, current_stack, current_stackbase
        );
    }

    printf("\nINTERRUPT STACKS\n");
    printf("NCPU STACK              STACKBASE\n");
    for (size_t i = 0; i < NCPU; i++) {
        current_stack -= i * DISTANCE;
        current_stackbase -= i * DISTANCE;
        printf("%-4d 0x%16lx 0x%16lx\n",
            i, current_stack, current_stackbase
        );
    }
}
