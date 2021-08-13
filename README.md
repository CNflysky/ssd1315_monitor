# SSD1315_monitor
[Simplified Chinese](https://github.com/CNflysky/ssd1315_monitor/blob/main/README_zh.md)  
![demo](https://github.com/CNflysky/ssd1315_monitor/blob/30b27b2acf07eabf27873023a94d65e6d676ec41/IMG_20210726_133237_1.jpg)  
A little oled panel for displaying performance data on SSD1315-based controller, written in C  
Use **hardware SPI** to connect the OLED module.  
Should be compatible with ssd1306 controllers.  
Only tested on Raspberry PI 4B.
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
## Setup software
Enter `raspi-config`,enable `SPI Interface` and exit.  
You may need to install the `gpiod` and `make` package  
`sudo apt install libgpiod-dev gpiod make`  
## Clone this repository  
`git clone https://github.com/CNflysky/ssd1315_monitor.git`  
## Compile and run  
`cd ssd1315_monitor && make && ./ssd1315`

# Customize
## Hardware Setup
open `config.conf`
```text
dc=18; #dc pin
reset=17; #reset pin
interface=eth0; #interface name
spidev=/dev/spidev0.0; #spidev file
gpiochip=gpiochip0; #chip which dc and reset are in
display_duration=5; #duration of a info page should be displayed and switch to next page

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
