# SSD1315_monitor

A little oled panel for displaying performance data on SSD1315-based OLED module, wirtten in C  
Use hardware SPI to connect the OLED module.  
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
Enter raspi-config,enable SPI Interface and exit.  
You may need to install the `gpiod` and `make` package  
`sudo apt install libgpiod-dev gpiod make`  
## Clone this repository  
`git clone https://github.com/CNflysky/ssd1315_monitor.git`  
## Compile it and run  
`cd ssd1315_monitor && make && ./ssd1315`

# Custom
TODO  
