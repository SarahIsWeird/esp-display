#include <stdio.h>

#include "esp_log.h"
#include "esp_sleep.h"

#include "lcd.h"
#include "util.h"
#include "protocol/comm.h"

static const char *TAG = "esp-display";

void app_main() {
    while (true) {
        int c = getc(stdin);

        if (c == -1) {
            delayMs(10); // TODO: Add deep sleep
            continue;
        }

        ungetc(c, stdin);

        handle_command();
        delayus(50);
    }

    ESP_ERROR_CHECK(lcd_init(LCD_CURSOR_OFF | LCD_BLINK_OFF));
    ESP_LOGI(TAG, "Successfully initialized LCD.");

    ESP_ERROR_CHECK(lcd_print("uwu"));
    ESP_ERROR_CHECK(lcd_set_cursor(0, 1));
    ESP_ERROR_CHECK(lcd_print("owo"));
    ESP_ERROR_CHECK(lcd_set_cursor(9, 2));
    ESP_ERROR_CHECK(lcd_print("<3"));

    ESP_LOGI(TAG, "Good night!");
    esp_deep_sleep_start();
}
