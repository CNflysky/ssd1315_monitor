#ifndef _FONT_H_
#define _FONT_H_
#include <stdint.h>
#include <string.h>
extern const uint8_t font_unifont_16[][16];
extern const uint8_t font_unifont_14[][14];
extern const uint8_t unifont_16_size;
extern const uint8_t unifont_14_size;
typedef enum { unifont_16, unifont_14 } fontname_t;
uint8_t font_get_width(fontname_t font);
uint8_t font_get_char(fontname_t font, uint8_t a, uint8_t b);

#endif