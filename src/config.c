#include "config.h"

uint64_t parse_config_int(const uint8_t *filepath, const uint8_t *name) {
  uint64_t ret = 0;
  FILE *fp = fopen(filepath, "r");
  bool success = false;
  uint8_t format[100];
  strcpy(format, name);
  strcat(format, "=%d");
  if (!fp) {
    perror(i18n("Open config file failed"));
    exit(1);
  }
  uint8_t buffer[100];
  while (!feof(fp)) {
    fgets(buffer, sizeof buffer, fp);
    if (sscanf(buffer, format, &ret) > 0) success = true;
  }
  fclose(fp);
  if (success)
    return ret;
  else
    return 0;
}

uint8_t *parse_config_string(const uint8_t *filepath, const uint8_t *name) {
  static uint8_t ret[100];
  FILE *fp = fopen(filepath, "r");
  bool success = false;
  uint8_t format[100];
  strcpy(format, name);
  strcat(format, "=%s");
  if (!fp) {
    perror(i18n("Open config file failed"));
    exit(1);
  }
  uint8_t buffer[100];
  while (!feof(fp)) {
    fgets(buffer, sizeof buffer, fp);
    if (sscanf(buffer, format, &ret) > 0) success = true;
  }
  fclose(fp);
  if (success)
    return ret;
  else
    return "Empty";
}

void check_config() {
  if (isdigit(dc) || dc == 0) {
    printf(i18n("Error:dc not specified or invalid:%d!\n"), dc);
    exit(1);
  }
  if (isdigit(reset) || reset == 0) {
    printf(i18n("Error:reset not specified or invalid:%d!\n"), reset);
    exit(1);
  }
  if (isdigit(display_duration) || display_duration == 0) {
    printf(i18n("Error:display_duration not specified or invalid:%d!\n"),
           display_duration);
    exit(1);
  }
  if (strlen(gpiochip) == 0 || !strcmp(gpiochip, "Empty")) {
    printf(i18n("Error:gpiochip not specified or invalid:%s!\n"), gpiochip);
    exit(1);
  }
  if (strlen(spidev) == 0 || !strcmp(spidev, "Empty")) {
    printf(i18n("Error:spidev not specified or invalid:%s!\n"), spidev);
    exit(1);
  }
  if (strlen(interface) == 0 || !strcmp(interface, "Empty")) {
    printf(i18n("Error:interface not specified or invalid:%s!\n"), interface);
    exit(1);
  }
  if (speed == 0) {
    printf(i18n("Error:speed not specified or invalid:%d!\n"), speed);
    exit(1);
  }
}

void get_conf(const uint8_t *path) {
  dc = parse_config_int(path, "dc");
  reset = parse_config_int(path, "reset");
  display_duration = parse_config_int(path, "display_duration");
  speed = parse_config_int(path, "speed");
  strcpy(interface, parse_config_string(path, "interface"));
  strcpy(spidev, parse_config_string(path, "spidev"));
  strcpy(gpiochip, parse_config_string(path, "gpiochip"));
}