#include <linux/sysinfo.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "ssd1315.h"
#include "spi.h"
#include "gpio.h"
#include "delay.h"
const uint8_t dc = 18;
const uint8_t reset = 17;
const uint8_t *interface = "eth0";
const uint8_t *spidev = "/dev/spidev0.0";
const uint8_t *gpiochip = "gpiochip0";

uint8_t *get_ip(const uint8_t *interface)
{
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    static uint8_t str[20];
    sprintf(str, "N:%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    printf("%s\n", str);
    return str;
}

uint8_t *get_mem()
{
    struct sysinfo info;
    sysinfo(&info);
    static uint8_t buf[11];
    sprintf(buf, "Mem: %d MB", (info.freeram / 1024 / 1024));
    printf("%s\n", buf);
    return buf;
}

uint64_t *get_load_status()
{
    FILE *fp = fopen("/proc/stat", "r");
    uint8_t buf[1024];
    fread(buf, sizeof buf, 1, fp);
    uint64_t vars[7];
    sscanf(buf, "cpu %d %d %d %d %d %d %d", &vars[0], &vars[1], &vars[2], &vars[3], &vars[4], &vars[5], &vars[6]);
    fclose(fp);
    uint64_t *data = (uint64_t *)calloc(2, sizeof(uint64_t));
    for (uint8_t i = 0; i < 7; i++)
    {
        data[0] += vars[i];
    }
    for (uint8_t i = 0; i < 3; i++)
    {
        data[1] += vars[i];
    }
    return data;
}

uint8_t *get_load(uint16_t rate)
{
    uint64_t *start = get_load_status();
    delay(rate);
    uint64_t *end = get_load_status();
    float percent = ((float)(end[1] - start[1])) / ((float)(end[0] - start[0])) * 100.0;
    free(end);
    free(start);
    static uint8_t str[20];
    sprintf(str, "CPU Load:%.2f%%", percent);
    printf("%s\n", str);
    return str;
}

uint8_t *get_boot_time()
{
    struct sysinfo info;
    sysinfo(&info);
    static uint8_t buf[11];
    uint8_t date[4] = {0}; //d,h,m,s
    uint64_t n = info.uptime;
    if (n > 86399)
    {
        date[0] = n / 86400;
        n %= 86400;
    }
    if (n >= 3600)
    {
        date[1] = info.uptime / 3600;
        n %= 3600;
    }
    if (n >= 60)
    {
        date[2] = n / 60;
        n %= 60;
    }
    date[3] = n;
    if (date[0] < 1)
    {
        sprintf(buf, "Uptime:%d:%d:%d", date[1], date[2], date[3]);
    }
    else
    {
        sprintf(buf, "Uptime:%dd,%dh", date[0], date[1]);
    }

    printf("%s\n", buf);
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
    printf("%s\n", str);
    return str;
}

uint8_t *get_processes()
{
    struct sysinfo info;
    sysinfo(&info);
    static uint8_t buf[11];
    sprintf(buf, "Processes:%d", info.procs);
    printf("%s\n", buf);
    return buf;
}

uint64_t get_network_status(const uint8_t *ifname, const uint8_t *txorrx)
{
    uint8_t path[512];
    sprintf(path, "/sys/class/net/%s/statistics/%s_bytes", ifname, txorrx);
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

uint8_t *get_network_speed(const uint8_t *ifname, const uint8_t *txorrx, uint16_t rate)
{
    uint8_t prefix[3];
    if (!strcmp(txorrx, "tx"))
        strcpy(prefix, "TX");
    else
        strcpy(prefix, "RX");
    long start = get_network_status(ifname, txorrx);
    delay(rate);
    long end = get_network_status(ifname, txorrx);
    static uint8_t str[20];
    float speed = (float)(end - start) / 1000.0; //KB/s
    if (speed > 1000)
        sprintf(str, "%s: %.3f MB/s", prefix, ((float)end - start) / 1000.0 / 1000.0);
    else
        sprintf(str, "%s: %.3f KB/s", prefix, ((float)end - start) / 1000.0);
    printf("%s\n", str);
    return str;
}

uint8_t *get_network_total()
{
    static uint8_t str[20];
    sprintf(str, "Total: %d MB", (get_network_status(interface, "tx") + get_network_status(interface, "rx")) / 1024 / 1024);
    printf("%s\n", str);
    return str;
}

void handle(int sig)
{
    printf("SIG %d Received,calling ssd1315_close()...\n", sig);
    ssd1315_close();
    exit(0);
}

void cpu_mem_temp_info_page()
{
    ssd1315_clear_buf();
    ssd1315_draw_rectangle(0, 0, 127, 63, false);
    ssd1315_draw_string(1, 1, "unifont_16", get_load(999));
    ssd1315_draw_string(1, 17, "unifont_16", get_mem());
    ssd1315_draw_string(1, 32, "unifont_16", get_boot_time());
    ssd1315_draw_string(1, 47, "unifont_16", get_core_temp());
    ssd1315_send_buf();
}

void network_info_page()
{
    ssd1315_clear_buf();
    ssd1315_draw_rectangle(0, 0, 127, 63, false);
    ssd1315_draw_string(1, 1, "unifont_16", get_ip(interface));
    ssd1315_draw_string(1, 17, "unifont_16", get_network_speed(interface, "tx", 500));
    ssd1315_draw_string(1, 32, "unifont_16", get_network_speed(interface, "rx", 500));
    ssd1315_draw_string(1, 47, "unifont_16", get_network_total());
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
    while (1)
    {
        uint8_t i = 0;
        do
        {
            cpu_mem_temp_info_page();
            i++;
        } while (i < 5);
        i = 0;
        do
        {
            network_info_page();
            i++;
        } while (i < 5);
    }
}
