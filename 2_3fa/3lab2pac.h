#ifndef LAB3PAC_H
#define LAB3PAC_H

#include <stddef.h>

typedef struct {
    const char *filename;
    size_t line;
    size_t char_index;
} Match;

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_FILE_OPEN,
    STATUS_ERR_FILE_READ,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t search_in_files(const char *substring, size_t file_count, Match **out_matches, size_t *out_count, ...);
void free_matches(Match *matches);

#endif