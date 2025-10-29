#include <stddef.h>
#include "string.h"

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t n) {
	unsigned char* dst = (unsigned char*)dstptr;
	const unsigned char* src = (const unsigned char*)srcptr;

#if defined(__x86_64__)
    while (n >= 8) {
        *(uint64_t*)dst = *(const uint64_t*)src;
        dst += 8;
        src += 8;
        n -= 8;
    }
#endif

#if defined(__i386__)
    while (n >= 4) {
        *(uint32_t*)dst = *(const uint32_t*)src;
        dst += 4;
        src += 4;
        n -= 4;
    }
#endif

	for (size_t i = 0; i < n; i++) {
		dst[i] = src[i];
	}

    return dstptr;
}
