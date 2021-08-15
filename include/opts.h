#ifndef _OPTS_H_
#define _OPTS_H_
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
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
void print_help(const char *name);
void parse_options(int argc, char **argv);
void kill_process();
#endif
