#ifndef _SPI_H_
#define _SPI_H_

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "gpio.h"
int spi_fd;
uint8_t spi_mode;
uint64_t spi_speed;
typedef enum { COMMAND, DATA } datatype_t;
void init_spi(const char *spidev_path, uint8_t mode, uint64_t speed);
void spi_write_1byte(datatype_t type, uint8_t data);
void spi_write_commands(uint8_t *data, uint16_t len);
void spi_write_bytes(uint8_t *data, uint16_t len);
void spi_fd_close();
#endif
