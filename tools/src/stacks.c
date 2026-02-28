#include <stdio.h>
#include <stdint.h>

#define STACK0      ((uint64_t)0xffffffffffffb000)
#define STACKBASE0  ((uint64_t)0xffffffffffffefff)
#define NCPU        (32)
#define DISTANCE    ((uint64_t)0x5000)

int main() {
    uint64_t current_stack = STACK0;
    uint64_t current_stackbase = STACKBASE0;

    printf("KERNEL STACKS\n");
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
