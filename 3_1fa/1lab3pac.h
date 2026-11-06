#ifndef LAB1_PAC3_H
#define LAB1_PAC3_H

#include <stddef.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t convert_to_base_2r(const char *num_str, int r, char **out_str);

#endif