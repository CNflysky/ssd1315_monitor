#ifndef _INFO_H_
#define _INFO_H_
#include <arpa/inet.h>
#include <errno.h>
#include <linux/sysinfo.h>
#include <net/if.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "delay.h"
uint8_t *get_ip(const uint8_t *interface);
uint8_t *get_mem();
uint64_t *get_load_status();
uint8_t *get_load(uint16_t rate);
uint8_t *get_boot_time();
uint8_t *get_core_temp();
uint8_t *get_processes();
uint64_t get_network_status(const uint8_t *ifname, const uint8_t *txorrx);
uint8_t *get_network_speed(const uint8_t *ifname, const uint8_t *txorrx,
                           uint16_t rate);
uint8_t *get_network_total(const uint8_t *interface);
#endif