#include "alloc/conversions.h"
#include "alloc/kpalloc.h"
#include "alloc/kvalloc.h"

physaddr_t physical(virtaddr_t virtual) {
    return (physaddr_t)virtual - hhdmoffset();
}

virtaddr_t virtual(physaddr_t physical) {
    return (virtaddr_t)(physical + hhdmoffset());
}
