#include <signal.h>

#include "config.h"
#include "delay.h"
#include "gpio.h"
#include "i18n.h"
#include "info.h"
#include "spi.h"
#include "ssd1315.h"
uint8_t dc;
uint8_t reset;
uint8_t interface[15];
uint8_t spidev[10];
uint8_t gpiochip[10];
uint8_t display_duration;
uint32_t speed;

void get_conf(const uint8_t *path) {
  dc = parse_config_int(path, "dc");
  reset = parse_config_int(path, "reset");
  display_duration = parse_config_int(path, "display_duration");
  speed = parse_config_int(path, "speed");
  strcpy(interface, parse_config_string(path, "interface"));
  strcpy(spidev, parse_config_string(path, "spidev"));
  strcpy(gpiochip, parse_config_string(path, "gpiochip"));
  if (dc == 0) {
    printf(i18n("Error:dc is empty!\n"));
    exit(1);
  }
  if (reset == 0) {
    printf(i18n("Error:reset is empty!\n"));
    exit(1);
  }
  if (display_duration == 0) {
    printf(i18n("Error:display_duration is empty!\n"));
    exit(1);
  }
  if (!strcmp(gpiochip, "Empty")) {
    printf(i18n("Error:gpiochip is empty!\n"));
    exit(1);
  }
  if (!strcmp(spidev, "Empty")) {
    printf(i18n("Error:spidev is empty!\n"));
    exit(1);
  }
  if (!strcmp(interface, "Empty")) {
    printf(i18n("Error:interface is empty!\n"));
    exit(1);
  }
}

void handle(int sig) {
  printf(i18n("SIG %d Received,calling ssd1315_close()...\n"), sig);
  ssd1315_close();
  exit(0);
}

void cpu_mem_temp_info_page()  // takes about 1s
{
  ssd1315_clear_buf();
  ssd1315_draw_rectangle(0, 0, 127, 63, false);
  ssd1315_draw_string(1, 1, unifont_16, get_load(999));
  ssd1315_draw_string(1, 17, unifont_16, get_mem());
  ssd1315_draw_string(1, 32, unifont_16, get_boot_time());
  ssd1315_draw_string(1, 47, unifont_16, get_core_temp());
  ssd1315_send_buf();
}

void network_info_page()  // take 1s too
{
  ssd1315_clear_buf();
  ssd1315_draw_rectangle(0, 0, 127, 63, false);
  ssd1315_draw_string(1, 1, unifont_16, get_ip(interface));
  ssd1315_draw_string(1, 17, unifont_16,
                      get_network_speed(interface, "tx", 500));
  ssd1315_draw_string(1, 32, unifont_16,
                      get_network_speed(interface, "rx", 500));
  ssd1315_draw_string(1, 47, unifont_16, get_network_total(interface));
  ssd1315_send_buf();
}

int main() {
  i18n_settings();
  get_conf("config.conf");
  signal(SIGINT, handle);
  signal(SIGTERM, handle);
  pin_reset = request_gpio(gpiochip, "pin_reset", reset);
  pin_dc = request_gpio(gpiochip, "pin_dc", dc);
  printf(i18n("Initializing SPI...\n"));
  init_spi(spidev, SPI_MODE_0, speed);
  printf(i18n("Initializing OLED...\n"));
  ssd1315_init();
  printf(i18n("Starting loop...\n"));
  uint8_t i = 0;
  while (1) {
    i = 0;
    do {
      cpu_mem_temp_info_page();
      i++;
    } while (i < display_duration);
    i = 0;
    do {
      network_info_page();
      i++;
    } while (i < display_duration);
  }
}
