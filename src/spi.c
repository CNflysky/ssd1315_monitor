#include "spi.h"
extern int spi_fd;
void init_spi(const char *spidev_path, uint8_t mode, uint64_t speed)
{
    uint8_t bitsperword = 8;
    spi_mode = mode;
    spi_speed = speed;
    int fd = open(spidev_path, O_RDWR);
    if (!fd)
    {
        fprintf(stderr, "Error:open spidev failed:%s!\n", strerror(errno));
        exit(1);
    }
    spi_fd = fd;
    if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode) < 0)
    {
        fprintf(stderr, "Error:set spi mode failed:%s!\n", strerror(errno));
        exit(1);
    }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsperword) < 0)
    {
        fprintf(stderr, "Error:set spi bit failed:%s!\n", strerror(errno));
        exit(1);
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0)
    {
        fprintf(stderr, "Error:set spi speed failed:%s!\n", strerror(errno));
        exit(1);
    }
}

void spi_write_1byte(datatype_t type, uint8_t data)
{
    uint8_t *buf = (uint8_t *)malloc(sizeof(uint8_t));
    *buf = data;
    gpiod_line_set_value(pin_dc, type);
    struct spi_ioc_transfer spi =
        {
            .tx_buf = (uint64_t)buf,
            .rx_buf = 0,
            .len = 1,
            .delay_usecs = 0,
            .bits_per_word = 8,
            .speed_hz = spi_speed};

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0)
    {
        fprintf(stderr, "Error:send data 0x%x with type %s failed:%s!\n", *buf, (type == 0) ? "COMMAND" : "DATA", strerror(errno));
        free(buf);
        exit(1);
    }
    else
        free(buf);
}

void spi_write_commands(uint8_t *data, uint16_t len)
{
    gpiod_line_set_value(pin_dc, COMMAND);
    struct spi_ioc_transfer spi =
        {
            .tx_buf = (uint64_t)data,
            .rx_buf = 0,
            .len = len,
            .delay_usecs = 0,
            .bits_per_word = 8,
            .speed_hz = spi_speed};

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0)
    {
        fprintf(stderr, "Error:send command set failed:%s!\n", strerror(errno));
        exit(1);
    }
}

void spi_write_bytes(uint8_t *data, uint16_t len)
{
    gpiod_line_set_value(pin_dc, DATA);
    struct spi_ioc_transfer spi =
        {
            .tx_buf = (uint64_t)data,
            .rx_buf = 0,
            .len = len,
            .delay_usecs = 0,
            .bits_per_word = 8,
            .speed_hz = spi_speed};

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0)
    {
        fprintf(stderr, "Error:send data set failed:%s!\n", strerror(errno));
        exit(1);
    }
}

void spi_fd_close()
{
    close(spi_fd);
}
