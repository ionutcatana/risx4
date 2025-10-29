#ifndef PSF_H
#define PSF_H 1

#include <stdint.h>

#define PSF1_MAGIC0 0x0436

struct psf_header {
    uint16_t magic;
    uint8_t mode;
    uint8_t char_size;
};

#endif
