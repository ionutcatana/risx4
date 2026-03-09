#include "lib/printf.h"
#include "limine.h"
#include "modules.h"
#include <stddef.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request modulereq = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = LIMINE_API_REVISION
};

void initmodules(void) {
    if (modulereq.response == NULL) {
        printf("no modules loaded\n");
        return;
    }

    for (size_t i = 0; i < modulereq.response->module_count; i++) {
        printf("module address: 0x%016llx\nmodule string: %s\n", modulereq.response->modules[i]->address, modulereq.response->modules[i]->string);
    }
}
