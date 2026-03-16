#include "alloc/kpalloc.h"
#include <stdint.h>

static uint64_t offset_val = 0;

void initoffset(const uint64_t offset)
{
    offset_val = offset;
}

uint64_t hhdmoffset(void)
{
    return offset_val;
}
