#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request addrreq = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 4
};
