#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>

uintptr_t physical(void* virtual) {
    return (uintptr_t)virtual - hhdmoffset();
}

void* virtual(uintptr_t physical) {
    return (void*)(physical + hhdmoffset());
}
