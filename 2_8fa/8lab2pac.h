#ifndef LAB8PAC_H
#define LAB8PAC_H

#include <stddef.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t sum_strings_in_base(int base, size_t count, char **out_sum, ...);
void free_sum(char *sum);

#endif