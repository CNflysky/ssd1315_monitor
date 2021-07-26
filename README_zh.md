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
TODO  
