#include <alloc/kpalloc.h>
#include <limine.h>

void initkvalloc(void) {
    return;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
uintptr_t allocpage(size_t count) {
    return 0;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void freepage(uintptr_t frameptr, size_t count) {
    return;
}
#pragma GCC diagnostic pop
