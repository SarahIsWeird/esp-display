#include "util.h"

#include "freertos/task.h"

void delayMs(int milliseconds) {
    vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}

void delayus(int microseconds) {
    vTaskDelay(microseconds / configTICK_RATE_HZ);
}