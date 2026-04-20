#ifndef ELFLOADER_H
#define ELFLOADER_H 1

#include "commonarch/paging.h"
#include "modules.h"
#include <stdint.h>

struct elfimage {
    uint64_t entry;                 // virtual entry point (e_entry)
    struct pagetable* pagetable;
    uint64_t stack;                 // top of user-mode stack
};

void loadelf(const struct module *mod, struct elfimage *out);

#endif
