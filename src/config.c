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