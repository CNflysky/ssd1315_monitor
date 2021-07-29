#include <signal.h>
#include "info.h"
#include "ssd1315.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"
const uint8_t dc = 18;
const uint8_t reset = 17;
const uint8_t *interface = "eth0";
const uint8_t *spidev = "/dev/spidev0.0";
const uint8_t *gpiochip = "gpiochip0";
const uint8_t display_duration = 5;

void handle(int sig)
{
    printf("SIG %d Received,calling ssd1315_close()...\n", sig);
    ssd1315_close();
    exit(0);
}

void cpu_mem_temp_info_page() // takes about 1s
{
    ssd1315_clear_buf();
    ssd1315_draw_rectangle(0, 0, 127, 63, false);
    ssd1315_draw_string(1, 1, "unifont_16", get_load(999));
    ssd1315_draw_string(1, 17, "unifont_16", get_mem());
    ssd1315_draw_string(1, 32, "unifont_16", get_boot_time());
    ssd1315_draw_string(1, 47, "unifont_16", get_core_temp());
    ssd1315_send_buf();
}

void network_info_page() // take 1s too
{
    ssd1315_clear_buf();
    ssd1315_draw_rectangle(0, 0, 127, 63, false);
    ssd1315_draw_string(1, 1, "unifont_16", get_ip(interface));
    ssd1315_draw_string(1, 17, "unifont_16", get_network_speed(interface, "tx", 500));
    ssd1315_draw_string(1, 32, "unifont_16", get_network_speed(interface, "rx", 500));
    ssd1315_draw_string(1, 47, "unifont_16", get_network_total(interface));
    ssd1315_send_buf();
}

int main()
{
    signal(SIGINT, handle);
    signal(SIGTERM, handle);
    pin_reset = request_gpio(gpiochip, "pin_reset", reset);
    pin_dc = request_gpio(gpiochip, "pin_dc", dc);
    printf("Initializing SPI...\n");
    init_spi(spidev, SPI_MODE_0, 125000000);
    printf("Initializing OLED...\n");
    ssd1315_init();
    printf("Starting loop...\n");
    uint8_t i = 0;
    while (1)
    {
        do
        {
            cpu_mem_temp_info_page();
            i++;
        } while (i < display_duration);
        i = 0;
        do
        {
            network_info_page();
            i++;
        } while (i < display_duration);
    }
}
