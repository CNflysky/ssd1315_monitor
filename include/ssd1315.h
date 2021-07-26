#ifndef _SSD1315_H_
#define _SSD1315_H_

#include <stdint.h>
#include <gpiod.h>
uint8_t display_buffer[128][8];
void ssd1315_init(struct gpiod_line *pin_reset);
void ssd1315_clear_buf();
void ssd1315_send_buf();
void ssd1315_draw_point(uint8_t x, uint8_t y, bool mode);
void ssd1315_fill_screen();
void ssd1315_clear_screen();
void ssd1315_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ssd1315_precircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y);
void ssd1315_draw_circle(uint16_t xc, uint16_t yc, uint16_t radius, bool filled);
void ssd1315_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool filled);
void ssd1315_draw_char_unifont_16(uint8_t x, uint8_t y, uint8_t ch, uint8_t mode);
void ssd1315_draw_string_unifont_16(uint8_t x, uint8_t y, uint8_t *str, uint8_t mode);
void ssd1315_close();
#endif
