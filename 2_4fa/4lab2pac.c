#include "4lab2pac.h"
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

void free_backup_result(BackupResult *result) {
    if (result == NULL) {
        return;
    }
    free(result->selected_indices);
    result->selected_indices = NULL;
    result->selected_count = 0;
    result->total_size = 0;
    result->total_importance = 0;
}

status_t select_files_for_backup(const File *files, size_t n, size_t capacity, BackupResult *result) {
    if (files == NULL || result == NULL) {
        return STATUS_ERR_NULL_ARG;
    }

    result->selected_indices = NULL;
    result->selected_count = 0;
    result->total_size = 0;
    result->total_importance = 0;

    if (n == 0 || capacity == 0) {
        return STATUS_OK;
    }

    if (n > SIZE_MAX - 1 || capacity > SIZE_MAX - 1) {
        return STATUS_ERR_OVERFLOW;
    }
    size_t rows = n + 1;
    size_t cols = capacity + 1;

    if (rows > SIZE_MAX / cols) {
        return STATUS_ERR_OVERFLOW;
    }
    size_t total_cells = rows * cols;

    if (total_cells > SIZE_MAX / sizeof(unsigned int)) {
        return STATUS_ERR_OVERFLOW;
    }

    unsigned int *dp = calloc(total_cells, sizeof(unsigned int));
    if (dp == NULL) {
        return STATUS_ERR_MEMORY;
    }

    char *taken = calloc(total_cells, sizeof(char));
    if (taken == NULL) {
        free(dp);
        return STATUS_ERR_MEMORY;
    }

    for (size_t i = 1; i <= n; i++) {
        for (size_t w = 1; w <= capacity; w++) {
            unsigned int not_take = dp[(i - 1) * cols + w];
            
            if (files[i - 1].size <= w) {
                size_t rem_w = w - files[i - 1].size;
                unsigned int prev_imp = dp[(i - 1) * cols + rem_w];
                
                if (files[i - 1].importance > UINT_MAX - prev_imp) {
                    free(dp);
                    free(taken);
                    return STATUS_ERR_OVERFLOW;
                }
                
                unsigned int take = prev_imp + files[i - 1].importance;
                
                if (take > not_take) {
                    dp[i * cols + w] = take;
                    taken[i * cols + w] = 1;
                } else {
                    dp[i * cols + w] = not_take;
                    taken[i * cols + w] = 0;
                }
            } else {
                dp[i * cols + w] = not_take;
                taken[i * cols + w] = 0;
            }
        }
    }

    size_t current_i = n;
    size_t current_w = capacity;
    size_t count = 0;
    
    result->total_size = 0;
    result->total_importance = dp[n * cols + capacity];

    while (current_i > 0 && current_w > 0) {
        if (taken[current_i * cols + current_w]) {
            count++;
            result->total_size += files[current_i - 1].size;
            current_w -= files[current_i - 1].size;
        }
        current_i--;
    }

    result->selected_count = count;
    result->selected_indices = NULL;

    if (count > 0) {
        result->selected_indices = malloc(count * sizeof(size_t));
        if (result->selected_indices == NULL) {
            free(dp);
            free(taken);
            return STATUS_ERR_MEMORY;
        }

        current_i = n;
        current_w = capacity;
        size_t idx = count;

        while (current_i > 0 && current_w > 0) {
            if (taken[current_i * cols + current_w]) {
                idx--;
                result->selected_indices[idx] = current_i - 1;
                current_w -= files[current_i - 1].size;
            }
            current_i--;
        }
    }

    free(dp);
    free(taken);
    return STATUS_OK;
}