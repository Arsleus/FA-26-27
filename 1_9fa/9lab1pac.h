#ifndef LAB9_LAB1PAC_H
#define LAB9_LAB1PAC_H

#include <stddef.h>

#define FIXED_SIZE 10

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t process_fixed_array(int *arr, size_t size, int *out_min, int *out_max);
status_t process_dynamic_arrays(size_t *out_size, int **out_array_c);
void free_dynamic_array(int *arr);

#endif