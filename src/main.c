#include <getopt.h>
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
uint8_t interface[20];
uint8_t spidev[20];
uint8_t gpiochip[20];
uint8_t display_duration;
uint32_t speed;
uint8_t config_path[255];
bool background;
bool use_config = false;

void kill_process() {
  int pid = 0;
  FILE *fp = fopen("/run/ssd1315.pid", "r");
  if (!fp) {
    perror(i18n("open pid file failed"));
    exit(1);
  }
  fscanf(fp, "%d\n", &pid);
  fclose(fp);
  remove("/run/ssd1315.pid");
  if (kill(pid, SIGTERM) < 0) {
    perror(i18n("kill process failed"));
    exit(1);
  } else
    exit(0);
}

void print_help(const char *name) {
  printf(
      i18n("Usage:%s [OPTION]... OR\n"
           "      %s -c [CONFIG_PATH]\n"
           "-d,--dc                    specify dc pin.\n"
           "-r,--reset                 specify reset pin.\n"
           "-i,--interface             specify network interface. e.g. eth0\n"
           "-s,--spidev                specify spi bus. e.g. /dev/spidev0.0\n"
           "-g,--gpiochip              specify gpiochip. e.g. gpiochip0\n"
           "-p,--display-duration      specify display_duration.\n"
           "-f,--speed                 specify spi speed.\n"
           "-c,--config                specify config file path.\n"
           "-b,--background            runs at background.\n"
           "-k,--kill                  kill a running background instance of "
           "this "
           "program.\n"
           "-h,--help                  print this message.\n"
           "-v,--version               print version.\n"
           "\n"
           "e.g. :ssd1315 -d 18 -r 17 -i eth0 -s /dev/spidev0.0 -g gpiochip0 "
           "-p 5 "
           "-f 125000000 \n"),
      name, name);
  exit(EXIT_SUCCESS);
}

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

void parse_options(int argc, char **argv) {
  int opt = 0;
  int opti = 0;
  if (argc == 1) print_help(argv[0]);
  struct option opts[] = {{"dc", required_argument, NULL, 'd'},
                          {"reset", required_argument, NULL, 'r'},
                          {"interface", required_argument, NULL, 'i'},
                          {"spidev", required_argument, NULL, 's'},
                          {"gpiochip", required_argument, NULL, 'g'},
                          {"display-duration", required_argument, NULL, 'p'},
                          {"speed", required_argument, NULL, 'f'},
                          {"config", required_argument, NULL, 'c'},
                          {"background", no_argument, NULL, 'b'},
                          {"help", no_argument, NULL, 'h'},
                          {"version", no_argument, NULL, 'v'},
                          {"kill", no_argument, NULL, 'k'},
                          {0, 0, 0, 0}

  };
  while ((opt = getopt_long(argc, argv, "d:r:i:s:g:p:f:c:bgvk", opts, &opti)) >
         0) {
    switch (opt) {
      case 'd':
        dc = atoi(optarg);
        break;
      case 'r':
        reset = atoi(optarg);
        break;
      case 'i':
        strncpy(interface, optarg, sizeof interface);
        break;
      case 's':
        strncpy(spidev, optarg, sizeof spidev);
        break;
      case 'g':
        strncpy(gpiochip, optarg, sizeof gpiochip);
        break;
      case 'p':
        display_duration = atoi(optarg);
        break;
      case 'f':
        speed = atol(optarg);
        break;
      case 'c':
        strncpy(config_path, optarg, sizeof config_path);
        use_config = true;
        break;
      case 'b':
        background = true;
        break;
      case 'v':
        printf("version:0.1\n");
        exit(EXIT_SUCCESS);
        break;
      case 'k':
        kill_process();
        break;
      case 'h':
      default:
        print_help(argv[0]);
        break;
    }
  }
}

int main(int argc, char **argv) {
  i18n_settings();
  parse_options(argc, argv);
  if (use_config) get_conf(config_path);
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
