#ifndef UTIL_H
#define UTIL_H

#include "freertos/FreeRTOS.h"

#define EXPECT_OK(STMT) { \
    esp_err_t err = STMT; \
    if (err != ESP_OK) { \
        return err; \
    } \
}

void delayMs(int milliseconds);
void delayus(int microseconds);

#endif
