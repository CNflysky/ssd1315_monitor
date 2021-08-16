# SSD1315_monitor
![demo](https://github.com/CNflysky/ssd1315_monitor/blob/30b27b2acf07eabf27873023a94d65e6d676ec41/IMG_20210726_133237_1.jpg)  
一块基于SSD1315驱动的OLED屏幕，用于显示系统信息  
OLED模块与开发板以**硬件SPI**的方式连接  
应该与SSD1306的OLED模块兼容.  
在树莓派4B与香橙派Zero 2上测试通过。  
# 如何使用  
## 连线:  
| SSD1315 PIN | RPI 4B |
| - | - |
| VCC | 3.3V |
| GND | 0V/GND |
| SCL | GPIO11 |
| SDA | GPIO10 |
| RES | GPIO17 |
| DC | GPIO18 |
| CS | GPIO8/GND |

| SSD1315 PIN | OPI Zero 2 |
| - | - |
| VCC | 3.3V |
| GND | 0V/GND |
| SCL | GPIO230 |
| SDA | GPIO231 |
| RES | GPIO69 |
| DC | GPIO70 |
| CS | GPIO233/GND |
## 软件设置  
进入`raspi-config`，启用SPI总线.  
你可能需要安装`gpiod`,`make`和`gettext`软件包  
`sudo apt install libgpiod-dev gpiod make gettext`  
## 克隆仓库  
`git clone https://github.com/CNflysky/ssd1315_monitor.git`  
## 编译并安装
`cd ssd1315_monitor && ./make_debian_package.sh rpi4b.conf && sudo dpkg -i ssd1315.deb`  
## 启动
两种方式启动:    
`ssd1315 -b -c /usr/local/etc/ssd1315/config.conf`  
如果你想让它停下来，就运行:  
`ssd1315 -k`  
或者用 `systemd` 服务来管理:  
`sudo systemctl start ssd1315.service` 和 `sudo systemctl stop ssd1315.service`.  
## 卸载
`sudo dpkg -r ssd1315`  
## 命令行用法  
```
用法:ssd1315 [选项]... 或者
      ssd1315 -c [配置文件]
-d,--dc                    指定 dc 引脚.
-r,--reset                 指定 reset 引脚.
-i,--interface             指定网卡. 示例: eth0
-s,--spidev                指定 spi 总线. 示例: /dev/spidev0.0
-g,--gpiochip              指定 gpiochip. 示例: gpiochip0
-p,--display-duration      指定 display_duration(显示间隔).
-f,--speed                 指定 spi 速度.
-c,--config                指定配置文件目录.
-b,--background            在后台运行.
-k,--kill                  杀死一个正在运行的本程序 实例.
-h,--help                  显示这个帮助.
-v,--version               显示版本信息.

使用示例 :ssd1315 -d 18 -r 17 -i eth0 -s /dev/spidev0.0 -g gpiochip0 -p 5 -f 125000000
```
你也可以通过运行`ssd1315 --help`命令来查看这些帮助.

# 自定义  
## 硬件设置
打开 `/usr/local/etc/ssd1315/config.conf`
```text
dc=18 #dc 引脚
reset=17 #reset 引脚
interface=eth0 #网络接口名
spidev=/dev/spidev0.0 #spidev文件
gpiochip=gpiochip0 #dc和reset所属的gpio芯片
display_duration=5 #每个页面的显示时间
```
## 添加页面  
打开 `src/info.c` 和 `src/info.h`,然后添加自定义功能:
```c
#ifndef _INFO_H_
#define _INFO_H_
#include <stdio.h>
char *display_hello_world();
#endif
```

```c
#include "info.h"
char *display_hello_world(){
  return "Hello,world";
}
```
然后在 `main.c` 中调用它:
```c
int main(){
...
ssd1315_draw_string(1,1,"unifont_16",display_hello_world());
...
}
```
与此同时我还预编写了一些别的功能:
```c
uint8_t *get_ip(const uint8_t *interface);
uint8_t *get_mem();
uint64_t *get_load_status();
uint8_t *get_load(uint16_t rate);
uint8_t *get_boot_time();
uint8_t *get_core_temp();
uint8_t *get_processes();
uint64_t get_network_status(const uint8_t *ifname, const uint8_t *txorrx);
uint8_t *get_network_speed(const uint8_t *ifname, const uint8_t *txorrx, uint16_t rate);
uint8_t *get_network_total(const uint8_t *interface);
```
它们能在 `info.h` 中找到.

