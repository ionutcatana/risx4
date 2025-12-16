#include <alloc/conversions.h>
#include <alloc/kvalloc.h>

uintptr_t physical(uintptr_t virtual) {
    return virtual - offset();
}

uintptr_t virtual(uintptr_t physical) {
    return physical + offset();
}
