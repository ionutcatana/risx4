#include <process.h>
#include <risx.h>
#include <arch/x86/registers.h>

#include <stdbool.h>

void schedule(void) {
//  printf("[CPU %lu] entered scheduler\n", readlapicid());
    while(true);
//  while(true)
//      printf("[CPU %lu] working! ! !\n", readlapicid());
}
