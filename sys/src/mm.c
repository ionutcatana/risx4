#include <alloc/allocator.h>
#include <mm.h>
#include <risx.h>

void initpmm(void) {
    initalloc();
}

void initvmm(void) {
    initalloc();    // I want this to be the virtual one...
}
