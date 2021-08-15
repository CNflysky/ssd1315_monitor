#include "config.h"
#include "delay.h"
#include "gpio.h"
#include "i18n.h"
#include "info.h"
#include "opts.h"
#include "spi.h"
#include "ssd1315.h"
uint8_t dc;
uint8_t reset;
uint8_t interface[20];
uint8_t spidev[20];
uint8_t gpiochip[20];
uint8_t display_duration;
uint32_t speed;
uint8_t config_path[255];
bool background;
bool use_config = false;

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

int main(int argc, char **argv) {
  i18n_settings();
  parse_options(argc, argv);
  if (use_config) get_conf(config_path);
  check_config();
  signal(SIGINT, handle);
  signal(SIGTERM, handle);
  pin_reset = request_gpio(gpiochip, "pin_reset", reset);
  pin_dc = request_gpio(gpiochip, "pin_dc", dc);
  printf(i18n("Initializing SPI...\n"));
  init_spi(spidev, SPI_MODE_0, speed);
  printf(i18n("Initializing OLED...\n"));
  ssd1315_init();
  if (background) {
    FILE *fp = fopen("/run/ssd1315.pid", "w");
    if (!fp) {
      perror(i18n("open pid file failed"));
      exit(1);
    }
    daemon(1, 0);
    fprintf(fp, "%d\n", getpid());
    fclose(fp);
  }
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
