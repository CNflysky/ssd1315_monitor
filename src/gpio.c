#include "gpio.h"

struct gpiod_line *request_gpio(const char *gpiochip, const char *pinname, int pin)
{
    struct gpiod_chip *chip = gpiod_chip_open_by_name(gpiochip);
    if (!chip)
    {
        fprintf(stderr, "Error:open chip failed:%s!\n", strerror(errno));
        exit(1);
    }
    struct gpiod_line *line = gpiod_chip_get_line(chip, pin);
    if (!line)
    {
        fprintf(stderr, "Error:get gpio line failed:%s!\n", strerror(errno));
        exit(1);
    }
    //mode ? gpiod_line_request_output(line, pinname, 0) : gpiod_line_request_input(line, pinname);
    if (gpiod_line_request_output(line, pinname, 0) < 0)
    {
        fprintf(stderr, "Error:set gpio to output failed:%s!\n", strerror(errno));
        exit(1);
    }
    return line;
}