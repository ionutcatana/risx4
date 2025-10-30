#ifndef PSF_H
#define PSF_H 1

#include <stddef.h>
#include <stdint.h>

#define PSF1_MAGIC 0x0436
#define PSF1_MODE512 0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE 0x05
#define PSF1_SEPARATOR 0xFFFF
#define PSF1_STARTSEQ 0xFFFE

#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define FONT_LENGTH 256

extern uintptr_t _binary_static_terminus_psf_start;
extern uintptr_t _binary_static_terminus_psf_end;
extern uintptr_t _binary_static_terminus_psf_size;

struct psf1_header {
    uint16_t magic;
    uint8_t mode;
    uint8_t char_size;
};

void font_init(uintptr_t psf1_header_addr);
void font_draw_char(size_t x, size_t y, char c,
                    uint8_t fg_r, uint8_t fg_g, uint8_t fg_b,
                    uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);

#endif
