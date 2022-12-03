#include "lcd.h"

#include "driver/i2c.h"
#include "esp_log.h"

#include "util.h"

static const char *TAG = "lcd";
static volatile uint8_t _backlight = 0;

static esp_err_t i2c_master_init() {
    int i2c_master_port = 0;
    
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &i2c_config);

    return i2c_driver_install(i2c_master_port,
                              i2c_config.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE,
                              0);
}

static esp_err_t lcd_expander_write(uint8_t byte) {
    return i2c_master_write_to_device(I2C_MASTER_NUM, I2C_LCD_ADDRESS, &byte, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

static esp_err_t lcd_pulse_enable(uint8_t byte) {
    EXPECT_WRITE(byte | LCD_ENABLE)
    delayus(1);

    EXPECT_WRITE(byte & ~LCD_ENABLE)
    delayus(50);

    return ESP_OK;
}

static esp_err_t lcd_expander_write_4bit(uint8_t nibble) {
    EXPECT_WRITE(nibble | _backlight)
    
    return lcd_pulse_enable(nibble | _backlight);
}

static esp_err_t lcd_send(uint8_t byte, uint8_t mode) {
    EXPECT_WRITE_4BIT((byte & 0xf0) | mode)
    
    return lcd_expander_write_4bit(((byte << 4) & 0xf0) | mode);
}

esp_err_t lcd_set_cursor(uint8_t x, uint8_t y) {
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    return lcd_send(LCD_SET_DDRAM_ADDRESS | (x + row_offsets[y]), 0);
}

esp_err_t lcd_init(uint8_t flags) {
    EXPECT_OK(i2c_master_init()); // We don't expect an OK here, because reinits fail.

    ESP_LOGI(TAG, "Waiting a bit for the expander to turn on");
    delayMs(10);

    ESP_LOGI(TAG, "Resetting expander and turning on backlight");
    EXPECT_WRITE(_backlight)
    delayMs(10);

    ESP_LOGI(TAG, "Setting 4 bit mode");

    EXPECT_WRITE_4BIT(0x30)
    delayMs(5);
    EXPECT_WRITE_4BIT(0x30)
    delayMs(5);
    EXPECT_WRITE_4BIT(0x30)
    delayus(150);

    EXPECT_WRITE_4BIT(0x20)

    ESP_LOGI(TAG, "Setting display settings");

    EXPECT_SEND(LCD_SET_FUNCTION | LCD_2LINE, 0)
    EXPECT_SEND(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | flags, 0)

    ESP_LOGI(TAG, "Clearing the display");
    EXPECT_SEND(LCD_CLEAR_DISPLAY, 0)
    delayMs(2);

    ESP_LOGI(TAG, "Resetting the cursor");
    EXPECT_SEND(LCD_RETURN_HOME, 0)
    delayMs(2);

    return ESP_OK;
}
 
esp_err_t lcd_print(const char *str) {
    for (int i = 0; str[i] != 0; i++) {
        EXPECT_SEND(str[i], LCD_REGISTER_SELECT)
    }

    return ESP_OK;
}

esp_err_t lcd_clear() {
    EXPECT_SEND(LCD_CLEAR_DISPLAY, 0)
    delayMs(2);

    return ESP_OK;
}

esp_err_t lcd_set_flags(uint8_t flags) {
    _backlight = (flags & 0x01) ? LCD_BACKLIGHT : 0;
    EXPECT_SEND(0, 0);

    uint8_t lcd_flags = LCD_DISPLAY_ON;

    if (flags & 0x02) {
        lcd_flags |= LCD_CURSOR_ON;
    }

    if (flags & 0x04) {
        lcd_flags |= LCD_BLINK_ON;
    }

    return lcd_send(LCD_DISPLAY_CONTROL | lcd_flags, 0);
}
