#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"

uint64_t physical(void* virtual) {
    return (uintptr_t)virtual - hhdmoffset();
}

void* virtual(uint64_t physical) {
    return (void*)(physical + hhdmoffset());
}
