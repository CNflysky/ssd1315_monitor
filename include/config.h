#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "i18n.h"
extern uint8_t dc;
extern uint8_t reset;
extern uint8_t interface[20];
extern uint8_t spidev[20];
extern uint8_t gpiochip[20];
extern uint8_t display_duration;
extern uint32_t speed;
extern uint8_t config_path[255];
extern bool background;
extern bool use_config;
void check_config();
uint64_t parse_config_int(const uint8_t *filepath, const uint8_t *name);
uint8_t *parse_config_string(const uint8_t *filepath, const uint8_t *name);
void get_conf(const uint8_t *path);
#endif