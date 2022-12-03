#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "esp_err.h"

#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22

#define I2C_MASTER_NUM 0
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_TIMEOUT_MS 1000

#define I2C_LCD_ADDRESS 0x27

#define LCD_SET_DDRAM_ADDRESS 0x80
#define LCD_SET_FUNCTION 0x20
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_RETURN_HOME 0x02
#define LCD_CLEAR_DISPLAY 0x01

#define LCD_DISPLAY_ON 0x04
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_REGISTER_SELECT 0x01

#define LCD_2LINE 0x08

#define EXPECT_WRITE(VALUE) EXPECT_OK(lcd_expander_write(VALUE))
#define EXPECT_WRITE_4BIT(VALUE) EXPECT_OK(lcd_expander_write_4bit(VALUE))
#define EXPECT_SEND(BYTE, MODE) EXPECT_OK(lcd_send(BYTE, MODE))

esp_err_t lcd_set_cursor(uint8_t x, uint8_t y);
esp_err_t lcd_init(uint8_t flags);
esp_err_t lcd_print(const char *str);
esp_err_t lcd_clear();
esp_err_t lcd_set_flags(uint8_t flags);

#endif
