#include <alloc/kpalloc.h>

#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request addrreq = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 4
};

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
