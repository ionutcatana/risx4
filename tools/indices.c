#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./indices $virtaddr\n");
        return 1;
    }

    if (strlen(argv[1]) != 16) {
        printf("Expected hexadecimal address of 16 characters\n");
        return 1;
    }

    uint64_t virtaddr = strtoull(argv[1], NULL, 16);
    printf("Indices:\nLVL4: %lu\nLVL3: %lu\nLVL2: %lu\nLVL1: %lu\n",
        (virtaddr >> 39) & 0x1ff,
        (virtaddr >> 30) & 0x1ff,
        (virtaddr >> 21) & 0x1ff,
        (virtaddr >> 12) & 0x1ff);

    return 0;
}
