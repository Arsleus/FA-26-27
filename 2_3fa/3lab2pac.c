#include "3lab2pac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define BLOCK_SIZE 512

void free_matches(Match *matches) {
    if (matches != NULL) {
        free(matches);
    }
}

static int find_substring(const char *text, size_t text_len, const char *pattern, size_t pattern_len, size_t *out_index) {
    if (pattern_len == 0 || text_len < pattern_len) {
        return 0;
    }

    for (size_t i = 0; i <= text_len - pattern_len; i++) {
        size_t j = 0;
        while (j < pattern_len && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == pattern_len) {
            *out_index = i;
            return 1;
        }
    }
    return 0;
}

status_t search_in_files(const char *substring, size_t file_count, Match **out_matches, size_t *out_count, ...) {
    if (substring == NULL || out_matches == NULL || out_count == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    size_t pattern_len = strlen(substring);
    if (pattern_len == 0 || file_count == 0) {
        return STATUS_ERR_INVALID_DATA;
    }

    Match *matches = NULL;
    size_t matches_size = 0;
    size_t matches_cap = 0;

    if (pattern_len > SIZE_MAX - BLOCK_SIZE) {
        return STATUS_ERR_OVERFLOW;
    }
    size_t buffer_size = BLOCK_SIZE + pattern_len - 1;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        return STATUS_ERR_MEMORY;
    }

    va_list args;
    va_start(args, out_count);

    for (size_t f = 0; f < file_count; f++) {
        const char *filename = va_arg(args, const char *);
        if (filename == NULL) {
            free(buffer);
            free(matches);
            va_end(args);
            return STATUS_ERR_NULL_ARG;
        }

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            free(buffer);
            free(matches);
            va_end(args);
            return STATUS_ERR_FILE_OPEN;
        }

        size_t line = 1;
        size_t ch = 1;
        size_t tail_len = 0;

        while (1) {
            size_t bytes_read = fread(buffer + tail_len, 1, BLOCK_SIZE, file);
            size_t total_len = tail_len + bytes_read;

            if (bytes_read == 0 && tail_len == 0) {
                break;
            }

            size_t processed = 0;
            while (processed + pattern_len <= total_len) {
                size_t found_idx;
                if (!find_substring(buffer + processed, total_len - processed, substring, pattern_len, &found_idx)) {
                    break;
                }
                found_idx += processed;

                for (size_t k = processed; k < found_idx; k++) {
                    if (buffer[k] == '\n') { line++; ch = 1; } else { ch++; }
                }
                processed = found_idx;

                if (processed + pattern_len > tail_len) {
                    if (matches_size == matches_cap) {
                        size_t new_cap = matches_cap == 0 ? 16 : matches_cap * 2;
                        if (new_cap > SIZE_MAX / sizeof(Match)) {
                            free(buffer); free(matches); fclose(file); va_end(args);
                            return STATUS_ERR_OVERFLOW;
                        }
                        Match *new_matches = realloc(matches, new_cap * sizeof(Match));
                        if (new_matches == NULL) {
                            free(buffer); free(matches); fclose(file); va_end(args);
                            return STATUS_ERR_MEMORY;
                        }
                        matches = new_matches;
                        matches_cap = new_cap;
                    }
                    matches[matches_size].filename = filename;
                    matches[matches_size].line = line;
                    matches[matches_size].char_index = ch;
                    matches_size++;
                }

                if (buffer[processed] == '\n') { line++; ch = 1; } else { ch++; }
                processed++;
            }

            size_t new_tail_len = (total_len < pattern_len - 1) ? total_len : (pattern_len - 1);
            size_t to_process = total_len - new_tail_len;

            for (size_t k = processed; k < to_process; k++) {
                if (buffer[k] == '\n') { line++; ch = 1; } else { ch++; }
            }

            if (new_tail_len > 0) {
                for (size_t k = 0; k < new_tail_len; k++) { 
                    buffer[k] = buffer[to_process + k];
                }
            }
            tail_len = new_tail_len;

            if (bytes_read == 0) {
                break;
            }
        }

        if (ferror(file)) {
            free(buffer);
            free(matches);
            fclose(file);
            va_end(args);
            return STATUS_ERR_FILE_READ;
        }
        fclose(file);
    }

    free(buffer);
    va_end(args);
    *out_matches = matches;
    *out_count = matches_size;
    return STATUS_OK;
}