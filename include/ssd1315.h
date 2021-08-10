#ifndef _SSD1315_H_
#define _SSD1315_H_

#include <gpiod.h>
#include <stdint.h>

#include "delay.h"
uint8_t display_buffer[128][8];
void ssd1315_init();
void ssd1315_clear_buf();
void ssd1315_send_buf();
void ssd1315_draw_point_with_mode(uint8_t x, uint8_t y, bool mode);
void ssd1315_draw_point(uint8_t x, uint8_t y);
void ssd1315_fill_screen();
void ssd1315_clear_screen();
void ssd1315_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ssd1315_precircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y);
void ssd1315_draw_circle(uint16_t xc, uint16_t yc, uint16_t radius,
                         bool filled);
void ssd1315_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            bool filled);
void ssd1315_draw_string(uint8_t x, uint8_t y, uint8_t *font, uint8_t *str);
void ssd1315_draw_char(uint8_t x, uint8_t y, uint8_t *font, uint8_t ch);
void ssd1315_close();
#endif
