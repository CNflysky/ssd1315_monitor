# SSD1315_monitor
[Simplified Chinese](https://github.com/CNflysky/ssd1315_monitor/blob/main/README_zh.md)  
![demo](https://github.com/CNflysky/ssd1315_monitor/blob/30b27b2acf07eabf27873023a94d65e6d676ec41/IMG_20210726_133237_1.jpg)  
A little oled panel for displaying performance data on SSD1315-based controller, written in C  
Use **hardware SPI** to connect the OLED module.  
Should be compatible with ssd1306 controllers.  
Tested on Raspberry PI 4B and OrangePi Zero 2.
# System version
RPI 4B:`2021-05-07-raspios-buster-arm64.img`  
OrangePi Zero 2:`Armbian_21.05.7_Orangepizero2_buster_legacy_4.9.279.img`  
# How to use
## Wire your module to the board:  
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
## Setup software
Enter `raspi-config`,enable `SPI Interface` and exit.  
You may need to install the `gpiod`,`make` and `gettext` package  
`sudo apt install libgpiod-dev gpiod make gettext`  
## Clone this repository  
`git clone https://github.com/CNflysky/ssd1315_monitor.git`  
## Compile and install 
for diffierent config file,update the `make_debian_package.sh`'s first option.  
e.g. `cd ssd1315_monitor && ./make_debian_package.sh rpi4b.conf && sudo dpkg -i ssd1315.deb`  
## Start 
I have provided two ways to run this program:  
`ssd1315 -b -c /usr/local/etc/ssd1315/config.conf`  
and when you want it to be stoped,just run:  
`ssd1315 -k`  
or use the `systemd` service:  
`sudo systemctl start ssd1315.service` and `sudo systemctl stop ssd1315.service`.  
## Uninstall
`sudo dpkg -r ssd1315`  
## command line usage  

```
Usage:./ssd1315 [OPTION]... OR  
      ./ssd1315 -c [CONFIG_PATH]  
-d,--dc                    specify dc pin.  
-r,--reset                 specify reset pin.  
-i,--interface             specify network interface. e.g. eth0  
-s,--spidev                specify spi bus. e.g. /dev/spidev0.0  
-g,--gpiochip              specify gpiochip. e.g. gpiochip0  
-p,--display-duration      specify display_duration.  
-f,--speed                 specify spi speed.  
-c,--config                specify config file path.  
-b,--background            runs at background.  
-k,--kill                  kill a running background instance of this program.  
-h,--help                  print this message.  
-v,--version               print version. 

e.g. :ssd1315 -d 18 -r 17 -i eth0 -s /dev/spidev0.0 -g gpiochip0 -p 5 -f 125000000  
```
You can also get this help by running the `ssd1315 --help` command.  

# Customize
## Hardware Setup
open `/usr/local/etc/ssd1315/config.conf`
```text
dc=18 #dc pin
reset=17 #reset pin
interface=eth0 #interface name
spidev=/dev/spidev0.0 #spidev file
gpiochip=gpiochip0 #chip which dc and reset are in
display_duration=5 #duration of a info page should be displayed and switch to next page

```
## Add your own page  
open `src/info.c` and `src/info.h`,then add your own function:
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
then call it at `main.c`:
```c
int main(){
...
ssd1315_draw_string(1,1,"unifont_16",display_hello_world());
...
}
```
and that's it.  
Also,I have provided some functions to use:
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
they have been decleared at `info.h`.
