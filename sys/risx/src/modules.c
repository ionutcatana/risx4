#include "alloc/kpalloc.h"
#include "commonarch/paging.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "limine.h"
#include "modules.h"

#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request modulereq = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = LIMINE_API_REVISION
};

static struct module modules[MAX_MODULES];
static size_t nmodules;

// copies limine modules into page-aligned frames
void
initmodules(void)
{
    if (modulereq.response == NULL)
        return;

    uint64_t count = modulereq.response->module_count;
    if (count > MAX_MODULES)
        count = MAX_MODULES;

    for (size_t i = 0; i < count; i++) {
        struct limine_file *file = modulereq.response->modules[i];
        size_t npages = (file->size + PAGE_SIZE - 1) / PAGE_SIZE;

        uint64_t phys = allocframe(npages);
        void *virt = virtual(phys);

        memcpy(virt, file->address, file->size);

        modules[i].physaddr = phys;
        modules[i].virtaddr = virt;
        modules[i].size     = file->size;
        modules[i].npages   = npages;
        modules[i].name     = file->path;

        printf("MODULE: Found \"%s\" (%zu pages)\n", modules[i].name, modules[i].npages);
    }

    nmodules = count;
}

const struct module *
getmodule(const char *name)
{
    for (size_t i = 0; i < nmodules; i++) {
        const char *a = modules[i].name;
        const char *b = name;

        while (*a != '\0' && *a == *b) {
            a++;
            b++;
        }
        if (*a == '\0' && *b == '\0')
            return (&modules[i]);
    }
    return (NULL);
}

size_t
modulecount(void)
{
    return (nmodules);
}

const struct module *
getmoduleidx(size_t idx)
{
    if (idx >= nmodules)
        return (NULL);
    return (&modules[idx]);
}
