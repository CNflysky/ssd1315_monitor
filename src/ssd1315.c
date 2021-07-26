#include "ssd1315.h"
#include "gpio.h"
#include "spi.h"
#include "font.h"
void ssd1315_init(struct gpiod_line *pin_reset)
{
    gpiod_line_set_value(pin_reset, 0);
    sleep(1);
    gpiod_line_set_value(pin_reset, 1);
    uint8_t init_seq[28]= {0xAE, 0x00, 0x10, 0x40, 0x81, 0xCF, 0xA1, 0xC8, 0xA6, 0xA8, 0x3F, 0xD3, 0x00, 0xD5, 0x80, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x40, 0x20, 0x02, 0x8D, 0x14, 0xA4, 0xA6};
    /*spi_write_1byte(COMMAND, 0xAE); //--turn off oled panel
    spi_write_1byte(COMMAND, 0x00); //---set low column address
    spi_write_1byte(COMMAND, 0x10); //---set high column address
    spi_write_1byte(COMMAND, 0x40); //--set start line address  Set Mapping RAM
                                    // Display Start Line (0x00~0x3F)
    spi_write_1byte(COMMAND, 0x81); //--set contrast control register
    spi_write_1byte(COMMAND, 0xCF); // Set SEG Output Current Brightness
    spi_write_1byte(COMMAND, 0xA1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    spi_write_1byte(COMMAND,
                    0xC8);          // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    spi_write_1byte(COMMAND, 0xA6); //--set normal display
    spi_write_1byte(COMMAND, 0xA8); //--set multiplex ratio(1 to 64)
    spi_write_1byte(COMMAND, 0x3f); //--1/64 duty
    spi_write_1byte(COMMAND,
                    0xD3);          //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
    spi_write_1byte(COMMAND, 0x00); //-not offset
    spi_write_1byte(COMMAND,
                    0xd5);          //--set display clock divide ratio/oscillator frequency
    spi_write_1byte(COMMAND, 0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    spi_write_1byte(COMMAND, 0xD9); //--set pre-charge period
    spi_write_1byte(COMMAND,
                    0xF1);          // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    spi_write_1byte(COMMAND, 0xDA); //--set com pins hardware configuration
    spi_write_1byte(COMMAND, 0x12);
    spi_write_1byte(COMMAND, 0xDB); //--set vcomh
    spi_write_1byte(COMMAND, 0x40); // Set VCOM Deselect Level
    spi_write_1byte(COMMAND, 0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    spi_write_1byte(COMMAND, 0x02); //// Set 页地址模式(A[1:0]=10b) 水平地址模式(A[1:0]=00b) 垂直地址模式(A[1:0]=01b)
    spi_write_1byte(COMMAND, 0x8D); //--set Charge Pump enable/disable
    spi_write_1byte(COMMAND, 0x14); //--set(0x10) disable
    spi_write_1byte(COMMAND, 0xA4); // Disable Entire Display On (0xa4/0xa5)
    spi_write_1byte(COMMAND, 0xA6); // Disable Inverse Display On (0xa6/a7)
    spi_write_1byte(COMMAND, 0xAF);*/
    spi_write_commands(init_seq, sizeof init_seq);
    ssd1315_clear_buf();
    ssd1315_send_buf();
    spi_write_1byte(COMMAND, 0xAF);
}

void ssd1315_clear_buf()
{
    memset(display_buffer, 0x00, 128 * 8);
}

void ssd1315_clear_screen()
{
    ssd1315_clear_buf();
}

void ssd1315_send_buf()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        spi_write_1byte(COMMAND, 0xb0 + i); //设置行起始地址
        spi_write_1byte(COMMAND, 0x00);     //设置低列起始地址
        spi_write_1byte(COMMAND, 0x10);     //设置高列起始地址
        for (uint8_t j = 0; j < 128; j++)
            spi_write_1byte(DATA, display_buffer[j][i]);
    }
}

void ssd1315_draw_point(uint8_t x, uint8_t y, bool mode)
{
    if (mode)
        display_buffer[x][y / 8] |= 1 << (y % 8);
    else
    {
        display_buffer[x][y / 8] = ~display_buffer[x][y / 8];
        display_buffer[x][y / 8] |= 1 << (y % 8);
        display_buffer[x][y / 8] = ~display_buffer[x][y / 8];
    }
}

void ssd1315_fill_screen()
{
    memset(display_buffer, 0xFF, 128 * 8);
}

void ssd1315_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    uint16_t xerr = 0, yerr = 0, delta_x = x2 - x1, delta_y = y2 - y1, distance;
    uint16_t incx, incy, uRow = x1, uCol = y1;

    if (delta_x > 0)
        incx = 1;
    else if (delta_x == 0)
        incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x;
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++)
    {
        ssd1315_draw_point(uRow, uCol, true);
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void ssd1315_draw_circle(uint16_t xc, uint16_t yc, uint16_t radius,
                         bool filled)
{
    int16_t x = 0, y = radius, yi, d = 3 - 2 * radius;
    if (filled)
    {
        while (x <= y)
        {
            for (yi = x; yi <= y; yi++)
                ssd1315_precircle(xc, yc, x, yi);
            if (d < 0)
                d = d + 4 * x + 6;
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
    else
    {
        while (x <= y)
        {
            ssd1315_precircle(xc, yc, x, y);
            if (d < 0)
                d = d + 4 * x + 6;
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

void ssd1315_precircle(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y)
{
    ssd1315_draw_point(xc + x, yc + y, true);
    ssd1315_draw_point(xc - x, yc + y, true);
    ssd1315_draw_point(xc + x, yc - y, true);
    ssd1315_draw_point(xc - x, yc - y, true);
    ssd1315_draw_point(xc + y, yc + x, true);
    ssd1315_draw_point(xc - y, yc + x, true);
    ssd1315_draw_point(xc + y, yc - x, true);
    ssd1315_draw_point(xc - y, yc - x, true);
}

void ssd1315_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool filled)
{
    //0,0 60,60
    if (filled)
    {
        for (uint8_t j = x1; j < x2; j++)
        {
            for (uint8_t i = y1; i < y2; i++)
            {
                ssd1315_draw_line(x1, i, x2, i);
            }
        }
    }
    else
    {
        ssd1315_draw_line(x1, y1, x2, y1);
        ssd1315_draw_line(x1, y1, x1, y2);
        ssd1315_draw_line(x1, y2, x2, y2);
        ssd1315_draw_line(x2, y1, x2, y2);
    }
}
void ssd1315_draw_char_unifont_16(uint8_t x, uint8_t y, uint8_t ch, uint8_t mode)
{
    uint8_t x0 = x, y0 = y;
    uint8_t size = 16;
    uint8_t size1 = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    uint8_t ch1 = ch - ' ';
    for (uint8_t i = 0; i < size1; i++)
    {
        uint8_t temp = font_unifont_16[ch1][i];
        for (uint8_t m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                ssd1315_draw_point(x, y, mode);
            else
                ssd1315_draw_point(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

void ssd1315_draw_string_unifont_16(uint8_t x, uint8_t y, uint8_t *str, uint8_t mode)
{
    while ((*str >= ' ') && (*str <= '~')) //判断是不是非法字符!
    {
        ssd1315_draw_char_unifont_16(x, y, *str, mode);
        x += 8;
        str++;
    }
}


void ssd1315_close(){
   ssd1315_clear_screen();
   ssd1315_send_buf();
   spi_write_1byte(COMMAND,0xAE);
   spi_fd_close();
   printf("close completed.\n");
}
