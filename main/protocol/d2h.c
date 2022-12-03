#include "d2h.h"

#include <stdio.h>
#include <string.h>

void comm_ready() {
    putchar(D2H_READY);
}

void comm_ack() {
    putchar(D2H_ACK);
}

void comm_err(comm_err_t error_code) {
    putchar(D2H_ERR);
    putchar(error_code);
}

void comm_log(const char *str) {
    printf("%u %hu %s\n", D2H_LOG, (unsigned short) strlen(str), str);
}
