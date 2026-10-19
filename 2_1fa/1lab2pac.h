#ifndef LAB2PAC_H
#define LAB2PAC_H
#include <stddef.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_FLAG,
    STATUS_ERR_EMPTY_STRING,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_SEED,
    STATUS_ERR_INTERNAL
} status_t;

status_t op_length(const char *s, size_t *out_len);
status_t op_reverse(const char *s, char **out);
status_t op_upper_odd(const char *s, char **out);
status_t op_partition(const char *s, char **out);
status_t parse_uint(const char *s, unsigned int *out);
status_t op_shuffle_concat(const char **strings, size_t count, unsigned int seed, char **out);

#endif