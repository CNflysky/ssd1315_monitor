#include "gpio.h"

struct gpiod_line *request_gpio(const char *gpiochip, const char *pinname,
                                int pin) {
  struct gpiod_chip *chip = gpiod_chip_open_by_name(gpiochip);
  if (!chip) {
    perror("Error:open chip failed");
    exit(1);
  }
  struct gpiod_line *line = gpiod_chip_get_line(chip, pin);
  if (!line) {
    perror("Error:get gpio line failed");
    exit(1);
  }
  if (gpiod_line_request_output(line, pinname, 0) < 0) {
    perror("Error:set gpio to output failed");
    exit(1);
  }
  return line;
}