#include "h2d.h"

#include <stdio.h>
#include <stdlib.h>

#include "../lcd.h"
#include "../util.h"

static int await_char() {
    while (1) {
        int c = getc(stdin);

        if (c == -1) {
            delayus(10);
            continue;
        }

        return c;
    }

    return -1;
}

void handle_command() {
    int command = getc(stdin);

    if (command == -1) {
        comm_err(ERR_INTERNAL);
        return;
    }

    comm_err_t error;

    switch (command) {
        case H2C_INIT:
            error = handle_init();
            break;
        case H2C_FLAGS:
            error = handle_flags();
            break;
        case H2C_CLEAR:
            error = handle_clear();
            break;
        case H2C_SET_CURSOR:
            error = handle_set_cursor();
            break;
        case H2C_PRINT:
            error = handle_print();
            break;
        default:
            error = ERR_INVALID_COMMAND;
            break;
    }

    if (error == ERR_NONE) {
        comm_ack();
        return;
    }

    comm_err(error);
}

comm_err_t handle_init() {
    comm_log("Initializing the display...");

    comm_err_t error = lcd_init(LCD_CURSOR_OFF | LCD_BLINK_OFF) == ESP_OK ? ERR_NONE : ERR_INTERNAL;

    comm_log(error == ERR_NONE ? "Initialized the display." : "Failed to initialize the display.");
    return error;
}

comm_err_t handle_flags() {
    await_char();

    comm_log("Display flags aren't implemented yet.");
    return ERR_NONE;
}

comm_err_t handle_clear() {
    return lcd_clear();
}

comm_err_t handle_set_cursor() {
    int x = await_char();
    int y = await_char();

    if (x < 0 || x > 19) {
        comm_log("Out of bounds cursor row.");
        return ERR_INVALID_ROW;
    }

    if (y < 0 || y > 3) {
        comm_log("Out of bounds cursor column.");
        return ERR_INVALID_COLUMN;
    }

    esp_err_t error = lcd_set_cursor(x, y);

    if (error != ESP_OK) {
        comm_log("Couldn't set new cursor position.");
        return ERR_INTERNAL;
    }

    comm_log("Successfully set the cursor position.");
    return ERR_NONE;
}

comm_err_t handle_print() {
    uint8_t length = await_char();
    char *buf = malloc(length + 1);

    for (int i = 0; i < length; i++) {
        buf[i] = await_char();
    }

    buf[length] = 0;

    lcd_print(buf);

    free(buf);

    return ERR_NONE;
}
