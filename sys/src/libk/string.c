#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int memcmp(const void* aptr, const void* bptr, size_t n) {
	const uint8_t* a = (const uint8_t*)aptr;
	const uint8_t* b = (const uint8_t*)bptr;
	for (size_t i = 0; i < n; i++) {
		if (a[i] < b[i]) {
			return -1;
		} else if (b[i] < a[i]) {
			return 1;
		}
	}

	return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t n) {
	uint8_t* dst = (uint8_t*)dstptr;
	const uint8_t* src = (const uint8_t*)srcptr;

    while (n >= 8) {
        *(uint64_t*)dst = *(const uint64_t*)src;
        dst += 8;
        src += 8;
        n -= 8;
    }

	for (size_t i = 0; i < n; i++) {
		dst[i] = src[i];
	}

    return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t n) {
	uint8_t* dst = (uint8_t*)dstptr;
	const uint8_t* src = (const uint8_t*)srcptr;
	if (dst < src) {
		for (size_t i = 0; i < n; i++) {
			dst[i] = src[i];
        }
	} else {
		for (size_t i = n; i != 0; i--) {
			dst[i-1] = src[i-1];
        }
	}

    return dstptr;
}

void* memset(void* bufptr, int value, size_t n) {
	uint8_t* buf = (uint8_t*)bufptr;
	for (size_t i = 0; i < n; i++) {
		buf[i] = (uint8_t) value;
    }

	return bufptr;
}

int strcmp(const char* s1, const char* s2) {
	size_t i = 0;
	while (true) {
		unsigned char a_byte = s1[i];
		unsigned char b_byte = s2[i];

        if(!a_byte && !b_byte) {
			return 0;
		}

		// If only one char is null, one of the following cases applies.
		if(a_byte < b_byte) {
			return -1;
        }
		if(a_byte > b_byte) {
			return 1;
        }

		i++;
	}
}

char* strcpy(char* restrict dst, const char* src) {
	char *dst_bytes = (char *)dst;
	char *src_bytes = (char *)src;
	while (*src_bytes) {
		*(dst_bytes++) = *(src_bytes++);
    }

	*dst_bytes = 0;

    return dst;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		len++;
    }

	return len;
}
