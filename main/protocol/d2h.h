#ifndef D2H_H
#define D2H_H

#include <stdint.h>

#define D2H_READY 0
#define D2H_ACK 1
#define D2H_ERR 2
#define D2H_LOG 3

typedef int comm_err_t;

#define ERR_NONE 0
#define ERR_INTERNAL 1
#define ERR_INVALID_ROW 2
#define ERR_INVALID_COLUMN 3
#define ERR_INVALID_COMMAND 4

void comm_ready();
void comm_ack();
void comm_err(comm_err_t error_code);
void comm_log(const char *str);

#endif
