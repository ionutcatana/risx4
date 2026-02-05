#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int i;
    if (argc < 2) {
        fprintf(stderr, "usage: kill pid...\n");
        exit(1);
    }

    for (i = 1; i < argc; i++)
        kill(atoi(argv[i]), SIGKILL);

    exit(0);
}
