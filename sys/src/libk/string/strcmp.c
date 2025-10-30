#include "libk/string.h"
#include <stdbool.h>
#include <stddef.h>

int strcmp(const char* a, const char* b) {
	size_t i = 0;
	while (true) {
		unsigned char a_byte = a[i];
		unsigned char b_byte = b[i];

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
