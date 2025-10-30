#include "libk/string.h"
#include <stddef.h>

char* strcpy(char* restrict dst, const char* src) {
	char *dst_bytes = (char *)dst;
	char *src_bytes = (char *)src;
	while (*src_bytes) {
		*(dst_bytes++) = *(src_bytes++);
    }

	*dst_bytes = 0;

    return dst;
}
