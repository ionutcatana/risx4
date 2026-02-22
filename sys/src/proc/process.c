#include <process.h>
#include <risx.h>
#include <arch/x86/registers.h>
#include <dlmalloc.h>

#include <stdbool.h>

void schedule(void) {
    uint64_t id = readlapicid();
    printf("[CPU %lu] entered scheduler\n", id);

    if (id == 0) {
        void* ptr1 = malloc(4096);
        if (ptr1 == NULL) {
            printf("[CPU %lu] failed to allocate memory.\n", id);
        } else {
            printf("[CPU %lu] allocated 4 KiB at 0x%016lx\n", id, (uintptr_t)ptr1);
            printf("[CPU %lu] freed memory.\n", id);
        }

        int* ptr2 = malloc(10 * sizeof(int));
        if (ptr2 == NULL) {
            printf("[CPU %lu] failed to allocate memory.\n", id);
        } else {
            for (size_t i = 0; i < 10; i++) {
                ptr2[i] = (int)i;
            }

            for (size_t i = 0; i < 10; i++) {
                printf("[CPU %lu] ptr2[%zu] = %d\n", id, i, ptr2[i]);
            }
            printf("[CPU %lu] allocated 10 integers at 0x%016lx\n", id, (uintptr_t)ptr2);
        }

        char* ptr3 = malloc(20 * sizeof(char));
        if (ptr3 == NULL) {
            printf("[CPU %lu] failed to allocate memory.\n", id);
        } else {
            strcpy(ptr3, "Hello, RISX!");
            printf("[CPU %lu] ptr3 = %s\n", id, ptr3);
            printf("[CPU %lu] allocated 20 characters at 0x%016lx\n", id, (uintptr_t)ptr3);
        }

        if (ptr1 != NULL) free(ptr1);
        if (ptr2 != NULL) free(ptr2);
        if (ptr3 != NULL) free(ptr3);
    }

    while(true);
}
