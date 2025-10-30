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

#define PSF2_MAGIC 0x864AB572
#define PSF2_HAS_UNICODE_TABLE 0x01
#define PSF2_MAXVERSION 0
#define PSF2_SEPARATOR 0xFF
#define PSF2_STARTSEQ 0xFE

extern uintptr_t _binary_static_terminus_psf_start;
extern uintptr_t _binary_static_terminus_psf_end;
extern size_t _binary_static_terminus_psf_size;

// char_size = height, width is constant 8
// length is constant 256 or 512 if PSF1_MODE512 is set
struct psf1_header {
    uint16_t magic;
    uint8_t mode;
    uint8_t char_size;
};

// char_size = height * ((width + 7) / 8)
struct psf2_header {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;   // offset of bitmaps in file
    uint32_t flags;
    uint32_t length;        // number of glyphs
    uint32_t char_size;     // number of bytes for each character
    uint32_t height;
    uint32_t width;         // max dimensions of glyphs
};

void font_init(uint8_t bitmap[], void* psf_header);
void access_bitmap();

// Get font dimensions
uint8_t font_get_width(void);
uint8_t font_get_height(void);

// Draw a character at position (x, y) with foreground/background colors
void font_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_r, uint8_t fg_g, uint8_t fg_b, uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);
#endif
