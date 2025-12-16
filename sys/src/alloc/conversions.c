#include <alloc/conversions.h>
#include <alloc/kpalloc.h>
#include <alloc/kvalloc.h>

uintptr_t physical(uintptr_t virtual) {
    return virtual - hhdmoffset();
}

uintptr_t virtual(uintptr_t physical) {
    return physical + hhdmoffset();
}
