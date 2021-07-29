#include "info.h"
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
        sprintf(str, "%s: %.2f MB/s", prefix, ((float)end - start) / 1000.0 / 1000.0);
    else
        sprintf(str, "%s: %.2f KB/s", prefix, ((float)end - start) / 1000.0);
    printf("%s\n", str);
    return str;
}

uint8_t *get_network_total(const uint8_t *interface)
{
    static uint8_t str[20];
    sprintf(str, "Total: %d MB", (get_network_status(interface, "tx") + get_network_status(interface, "rx")) / 1024 / 1024);
    printf("%s\n", str);
    return str;
}
