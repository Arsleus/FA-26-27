#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>
#include "4lab2pac.h"

static char *read_word(void) {
    int c;
    while ((c = fgetc(stdin)) != EOF && isspace(c)) {
    }
    if (c == EOF) {
        return NULL;
    }

    size_t cap = 16;
    size_t len = 0;
    char *buf = malloc(cap);
    if (buf == NULL) {
        return NULL;
    }

    while (c != EOF && !isspace(c)) {
        if (len + 1 >= cap) {
            if (cap > SIZE_MAX / 2) {
                free(buf);
                return NULL;
            }
            cap *= 2;
            char *new_buf = realloc(buf, cap);
            if (new_buf == NULL) {
                free(buf);
                return NULL;
            }
            buf = new_buf;
        }
        buf[len++] = (char)c;
        c = fgetc(stdin);
    }
    buf[len] = '\0';
    return buf;
}

static int parse_uint(const char *s, unsigned int *out) {
    if (s == NULL || s[0] == '\0') return 0;
    unsigned int result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
        unsigned int digit = (unsigned int)(s[i] - '0');
        if (result > (UINT_MAX - digit) / 10) {
            return 0;
        }
        result = result * 10 + digit;
    }
    *out = result;
    return 1;
}

static int parse_size_t(const char *s, size_t *out) {
    if (s == NULL || s[0] == '\0') return 0;
    size_t result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
        size_t digit = (size_t)(s[i] - '0');
        if (result > (SIZE_MAX - digit) / 10) {
            return 0;
        }
        result = result * 10 + digit;
    }
    *out = result;
    return 1;
}

static void free_files_array(File *files, size_t count) {
    if (files == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(files[i].name);
    }
    free(files);
}

int main(void) {
    char *n_str = read_word();
    char *cap_str = read_word();

    if (n_str == NULL || cap_str == NULL) {
        fprintf(stderr, "Ошибка: не удалось прочитать n и capacity.\n");
        free(n_str);
        free(cap_str);
        return 1;
    }

    size_t n = 0;
    size_t capacity = 0;

    if (!parse_size_t(n_str, &n)) {
        fprintf(stderr, "Ошибка: n должно быть целым неотрицательным числом.\n");
        free(n_str);
        free(cap_str);
        return 1;
    }
    free(n_str);

    if (!parse_size_t(cap_str, &capacity)) {
        fprintf(stderr, "Ошибка: capacity должно быть целым неотрицательным числом.\n");
        free(cap_str);
        return 1;
    }
    free(cap_str);

    if (n == 0) {
        fprintf(stderr, "Ошибка: количество файлов должно быть больше нуля.\n");
        return 1;
    }

    if (n > SIZE_MAX / sizeof(File)) {
        fprintf(stderr, "Ошибка: слишком много файлов.\n");
        return 1;
    }

    File *files = malloc(n * sizeof(File));
    if (files == NULL) {
        fprintf(stderr, "Ошибка: недостаточно памяти под массив файлов.\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        char *name = read_word();
        char *size_str = read_word();
        char *imp_str = read_word();

        if (name == NULL || size_str == NULL || imp_str == NULL) {
            fprintf(stderr, "Ошибка: неожиданный конец ввода.\n");
            free(name);
            free(size_str);
            free(imp_str);
            free_files_array(files, i);
            return 1;
        }

        unsigned int size = 0;
        unsigned int importance = 0;

        if (!parse_uint(size_str, &size) || !parse_uint(imp_str, &importance)) {
            fprintf(stderr, "Ошибка: размер и важность должны быть неотрицательными целыми числами.\n");
            free(name);
            free(size_str);
            free(imp_str);
            free_files_array(files, i);
            return 1;
        }
        free(size_str);
        free(imp_str);

        if (size == 0) {
            fprintf(stderr, "Ошибка: размер файла должен быть больше нуля.\n");
            free(name);
            free_files_array(files, i);
            return 1;
        }

        for (size_t j = 0; j < i; j++) {
            if (strcmp(files[j].name, name) == 0) {
                fprintf(stderr, "Ошибка: имена файлов должны быть уникальными.\n");
                free(name);
                free_files_array(files, i);
                return 1;
            }
        }

        files[i].name = name;
        files[i].size = size;
        files[i].importance = importance;
    }

    BackupResult result = {0};
    status_t st = select_files_for_backup(files, n, capacity, &result);

    if (st != STATUS_OK) {
        if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти для вычислений.\n");
        } else if (st == STATUS_ERR_OVERFLOW) {
            fprintf(stderr, "Ошибка: переполнение при вычислениях.\n");
        } else {
            fprintf(stderr, "Ошибка выполнения (код %d).\n", st);
        }
        free_files_array(files, n);
        return 1;
    }

    printf("TOTAL_SIZE %u\n", result.total_size);
    printf("TOTAL_IMPORTANCE %u\n", result.total_importance);
    printf("FILES %zu\n", result.selected_count);

    for (size_t i = 0; i < result.selected_count; i++) {
        size_t original_idx = result.selected_indices[i];
        printf("%s\n", files[original_idx].name);
    }

    free_backup_result(&result);
    free_files_array(files, n);

    return 0;
}