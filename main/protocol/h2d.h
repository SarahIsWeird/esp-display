#ifndef H2C_H
#define H2C_H

#include "esp_err.h"

#include "d2h.h"

#define H2C_INIT 0
#define H2C_FLAGS 1
#define H2C_CLEAR 2
#define H2C_SET_CURSOR 3
#define H2C_PRINT 4

#define FLAG_BACKLIGHT 1
#define FLAG_CURSOR 2
#define FLAG_BLINK 3
#define FLAG_LINE_BREAK 4

void handle_command();

comm_err_t handle_init();
comm_err_t handle_flags();
comm_err_t handle_clear();
comm_err_t handle_set_cursor();
comm_err_t handle_print();

#endif
