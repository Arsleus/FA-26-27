#ifndef LAB4PAC_H
#define LAB4PAC_H

#include <stddef.h>

typedef struct {
    char *name;
    unsigned int size;
    unsigned int importance;
} File;

typedef struct {
    size_t *selected_indices;
    size_t selected_count;
    unsigned int total_size;
    unsigned int total_importance;
} BackupResult;

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t select_files_for_backup(const File *files, size_t n, size_t capacity, BackupResult *result);
void free_backup_result(BackupResult *result);

#endif