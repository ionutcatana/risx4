#include <alloc/bitmap.h>

void setbit(uint32_t* bitmap, size_t idx) {
    bitmap[idx / 32] |= (1 << (idx % 32));
}

void unsetbit(uint32_t* bitmap, size_t idx) {
    bitmap[idx / 32] &= ~(1 << (idx % 32));
}

bool checkbit(uint32_t* bitmap, size_t idx) {
    return bitmap[idx / 32] & (1 << (idx % 32));
}
