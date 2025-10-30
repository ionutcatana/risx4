#include "libk/string.h"
#include <stddef.h>

void* memset(void* bufptr, int value, size_t n) {
	unsigned char* buf = (unsigned char*)bufptr;
	for (size_t i = 0; i < n; i++) {
		buf[i] = (unsigned char) value;
    }

	return bufptr;
}
