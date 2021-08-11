#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i18n.h"
uint64_t parse_config_int(const uint8_t *filepath, const uint8_t *name);
uint8_t *parse_config_string(const uint8_t *filepath, const uint8_t *name);
#endif