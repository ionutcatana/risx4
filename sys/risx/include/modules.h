#ifndef MODULES_H
#define MODULES_H 1

#include "risx/config.h"
#include <stddef.h>
#include <stdint.h>

struct module {
	uint64_t	physaddr;   // page-aligned physical base
	void		*virtaddr;  // hhdm virtual base
	size_t		size;	    // original file size in bytes
	size_t		npages;	    // number of allocated pages
	const char	*name;	    // module path string (from bootloader)
};

void   initmodules(void);
size_t modulecount(void);

const struct module	*getmodule(const char *name);
const struct module	*getmoduleidx(size_t idx);

#endif
