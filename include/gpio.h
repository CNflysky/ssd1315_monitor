#ifndef _GPIO_H_
#define _GPIO_H_
#include <errno.h>
#include <gpiod.h>
#include <stdio.h>
#include <string.h>
struct gpiod_line *pin_dc;
struct gpiod_line *pin_reset;
struct gpiod_line *request_gpio(const char *gpiochip, const char *pinname,
                                int pin);
#endif