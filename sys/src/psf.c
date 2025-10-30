#include <stddef.h>
#include <stdint.h>
#include "psf.h"
#include "stdlib.h"

void font_init(uint8_t bitmap[], void* psf_header) {
    struct psf1_header* header = (struct psf1_header*)psf_header;
    if (header->magic != PSF1_MAGIC) {
        // only PSF1 is supported for now
        abort();
    }

    size_t n = 256;
    if (header->mode & PSF1_MODE512) {
        n = 512;
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < header->char_size; j++) {
            bitmap[i * header->char_size + j] =
                *((uint8_t*)psf_header + sizeof(struct psf1_header) + i * header->char_size + j);
        }
    }
}
