# SSD1315_monitor
![demo](https://github.com/CNflysky/ssd1315_monitor/blob/30b27b2acf07eabf27873023a94d65e6d676ec41/IMG_20210726_133237_1.jpg)  
一块基于SSD1315驱动的OLED屏幕，用于显示系统信息  
OLED模块与开发板以**硬件SPI**的方式连接  
应该与SSD1306的OLED模块兼容.  
只在树莓派4B上测试通过。  
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
## 软件设置  
进入`raspi-config`，启用SPI总线.  
你可能需要安装`gpiod`与`make`软件包  
`sudo apt install libgpiod-dev gpiod make`  
## 克隆仓库  
`git clone https://github.com/CNflysky/ssd1315_monitor.git`  
## 编译并运行  
`cd ssd1315_monitor && make && ./ssd1315`

# 自定义  
## 硬件设置
打开 `src/main.c`
```c
const uint8_t dc = 18; //dc 引脚
const uint8_t reset = 17; //reset 引脚
const uint8_t *interface = "eth0"; //网络接口名
const uint8_t *spidev = "/dev/spidev0.0"; //spidev文件
const uint8_t *gpiochip = "gpiochip0"; //dc和reset所属的gpio芯片
const uint8_t display_duration = 5; //每个页面的显示时间
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

