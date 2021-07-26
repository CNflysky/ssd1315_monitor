#include "ssd1315.h"
#include "spi.h"
#include "gpio.h"
#include <linux/sysinfo.h>
#include <sys/sysinfo.h>
#include <signal.h>

const uint8_t dc = 18;
const uint8_t reset = 17;
const uint8_t *interface = "eth0";
const uint8_t *spidev = "/dev/spidev0.0";
const uint8_t *gpiochip = "gpiochip0";

uint8_t *get_hostname()
{
    static uint8_t host[20];
    uint8_t str[20];
    gethostname(str, sizeof str);
    sprintf(host, "Host:%s", str);
    printf("%s\n",str);
    return host;
}
uint8_t *get_mem()
{
    struct sysinfo info;
    sysinfo(&info);
    static uint8_t buf[11];
    sprintf(buf, "Mem: %d MB", (info.freeram / 1024 / 1024));
    printf("%s\n",buf);
    return buf;
}

uint8_t *get_core_temp()
{
    uint8_t temp[10];
    static uint8_t str[10];
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp)
    {
        fprintf(stderr, "Error:open thermal file failed:%s\n", strerror(errno));
        exit(1);
    }
    fgets(temp, sizeof temp, fp);
    fclose(fp);
    sprintf(str, "CPU0 Temp: %d'C", atoi(temp) / 1000);
    printf("%s\n",str);
    return str;
}

uint8_t *get_processes()
{
    struct sysinfo info;
    sysinfo(&info);
    static uint8_t buf[11];
    sprintf(buf, "Processes:%d", info.procs);
    printf("%s\n",buf);
    return buf;
}

uint64_t get_network_rx_status(const uint8_t *ifname)
{
    uint8_t path[512];
    sprintf(path, "/sys/class/net/%s/statistics/rx_bytes", ifname);
    FILE *fp = fopen(path, "r");
    uint8_t buf[1024];
    long speed;
    if (!fp)
    {
        fprintf(stderr, "Error:open network file failed:%s\n", strerror(errno));
        exit(1);
    }
    fread(buf, 1, sizeof buf, fp);
    fclose(fp);
    sscanf(buf, "%ld", &speed);
    return speed;
}

uint8_t *get_network_rx_speed(const uint8_t *ifname)
{
    long start = get_network_rx_status(ifname);
    sleep(1);
    long end = get_network_rx_status(ifname);
    static uint8_t str[20];
    float speed = (float)(end - start) / 1000.0; //KB/s
    if (speed > 1000)
        sprintf(str, "RX: %.3f MB/s", ((float)end - start) / 1000.0 / 1000.0);
    else
        sprintf(str, "RX: %.3f KB/s", ((float)end - start) / 1000.0);
    printf("%s\n",str);
    return str;
}

uint64_t get_network_tx_status(const uint8_t *ifname)
{
    uint8_t path[512];
    sprintf(path, "/sys/class/net/%s/statistics/tx_bytes", ifname);
    FILE *fp = fopen(path, "r");
    uint8_t buf[1024];
    long speed;
    if (!fp)
    {
        fprintf(stderr, "Error:open network file failed:%s\n", strerror(errno));
        exit(1);
    }
    fread(buf, 1, sizeof buf, fp);
    fclose(fp);
    sscanf(buf, "%ld", &speed);
    return speed;
}

uint8_t *get_network_tx_speed(const uint8_t *ifname)
{
    long start = get_network_tx_status(ifname);
    sleep(1);
    long end = get_network_tx_status(ifname);
    static uint8_t str[20];
    float speed = (float)(end - start) / 1000.0; //KB/s
    if (speed > 1000)
        sprintf(str, "TX: %.3f MB/s", ((float)end - start) / 1000.0 / 1000.0);
    else
        sprintf(str, "TX: %.3f KB/s", ((float)end - start) / 1000.0);
    printf("%s\n",str);
    return str;
}

void shutdown(int sig)
{
    printf("SIG %d Received,calling ssd1315_close()...\n", sig);
    ssd1315_close();
    exit(0);
}

int main()
{
    signal(SIGINT, shutdown);
    pin_reset = request_gpio(gpiochip, "pin_reset", reset);
    pin_dc = request_gpio(gpiochip, "pin_dc", dc);
    printf("Initializing SPI...\n");
    init_spi(spidev, SPI_MODE_0, 125000000);
    printf("Initializing OLED...\n");
    ssd1315_init(pin_reset);
    printf("Starting loop...\n");
    while (1)
    {
        ssd1315_clear_buf();
        ssd1315_draw_string_unifont_16(0, 0, get_core_temp(), true);
        ssd1315_draw_string_unifont_16(0, 16, get_mem(), true);
        ssd1315_draw_string_unifont_16(0, 32, get_network_rx_speed(interface), true);
        ssd1315_draw_string_unifont_16(0, 48, get_network_tx_speed(interface), true);
        //ssd1315_draw_string_unifont_16(0, 48, get_processes(), true);
        ssd1315_send_buf();
        sleep(1);
    }
}
