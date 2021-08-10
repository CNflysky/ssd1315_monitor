#include "i18n.h"
uint8_t* i18n(uint8_t* str) { return gettext(str); }
void i18n_settings() {
  setlocale(LC_ALL, "");
  bindtextdomain("ssd1315", "/usr/share/locale/");
  textdomain("ssd1315");
}