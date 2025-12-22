#include <risx.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

noreturn void risx(void) {
    printf("entered RISX\n");
//  enumeratecpus();

//  printf("\033[31mThis text is red!\033[0m\n");
//  printf("This text is not red\n");
    while(true);
    panic("unexpected return from scheduler.\n");
}


