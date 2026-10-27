#ifndef LAB9PAC_H
#define LAB9PAC_H

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

status_t check_fractions(int base, size_t count, double epsilon, int **out_results, ...);
void free_results(int *results);

#endif