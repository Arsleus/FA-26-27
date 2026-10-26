#include "1lab2pac.h"
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static size_t my_strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

status_t op_length(const char *s, size_t *out_len) {
    if (s == NULL || out_len == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (s[0] == '\0') {
        return STATUS_ERR_EMPTY_STRING;
    }
    *out_len = my_strlen(s);
    return STATUS_OK;
}

status_t op_reverse(const char *s, char **out) {
    if (s == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (s[0] == '\0') {
        return STATUS_ERR_EMPTY_STRING;
    }
    size_t len = my_strlen(s);
    if (len > SIZE_MAX - 1) {
        return STATUS_ERR_OVERFLOW;
    }
    char *res = (char*)malloc(len + 1);
    if (res == NULL) {
        return STATUS_ERR_MEMORY;
    }
    for (size_t i = 0; i < len; i++) {
        res[i] = s[len - 1 - i];
    }
    res[len] = '\0';
    *out = res;    
    return STATUS_OK;
}

status_t op_upper_odd(const char *s, char **out) {
    if (s == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (s[0] == '\0') {
        return STATUS_ERR_EMPTY_STRING;
    }
    size_t len = my_strlen(s);
    if (len > SIZE_MAX - 1) {
        return STATUS_ERR_OVERFLOW;
    }
    char *res = (char*)malloc(len + 1);
    if (res == NULL) {
        return STATUS_ERR_MEMORY;
    }
    for (size_t i = 0; i < len; i++) {
        char c = s[i];
        if (i % 2 != 0 && c >= 'a' && c <= 'z') {
            res[i] = (char)(c - 'a' + 'A');
        } else {
            res[i] = c;
        }
    }
    res[len] = '\0';
    *out = res;
    return STATUS_OK;
}

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

static int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

status_t op_partition(const char *s, char **out) {
    if (s == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (s[0] == '\0') {
        return STATUS_ERR_EMPTY_STRING;
    }
    size_t len = my_strlen(s);
    if (len > SIZE_MAX - 1) {
        return STATUS_ERR_OVERFLOW;
    }
    char *res = (char*)malloc(len + 1);
    if (res == NULL) {
        return STATUS_ERR_MEMORY;
    }
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        if (is_digit(s[i])) {
            res[k++] = s[i];
        }
    }
    for (size_t i = 0; i < len; i++) {
        if (is_letter(s[i])) {
            res[k++] = s[i];
        }
    }
    for (size_t i = 0; i < len; i++) {
        if (!is_digit(s[i]) && !is_letter(s[i])) {
            res[k++] = s[i];
        }
    }
    res[len] = '\0';
    *out = res;
    return STATUS_OK;
}

status_t parse_uint(const char *s, unsigned int *out) {
    if (s == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (s[0] == '\0') {
        return STATUS_ERR_EMPTY_STRING;
    }
    unsigned int result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return STATUS_ERR_SEED; 
        }
        unsigned int digit = (unsigned int)(s[i] - '0');
        if (result > (UINT_MAX - digit) / 10) {
            return STATUS_ERR_OVERFLOW;
        }
        result = result * 10 + digit;
    }
    *out = result;
    return STATUS_OK;
}

status_t op_shuffle_concat(const char **strings, size_t count, unsigned int seed, char **out) {
    if (strings == NULL || out == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (count == 0) {
        return STATUS_ERR_EMPTY_STRING;
    }
    size_t total_len = 0;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            return STATUS_ERR_NULL_ARG;
        }
        size_t len = my_strlen(strings[i]);
        if (total_len > SIZE_MAX - len) {
            return STATUS_ERR_OVERFLOW;
        }
        total_len += len;
    }
    if (total_len > SIZE_MAX - 1) {
        return STATUS_ERR_OVERFLOW;
    }
    char *res = (char*)malloc(total_len + 1);
    if (res == NULL) {
        return STATUS_ERR_MEMORY;
    }
    srand(seed);
    for (size_t i = count - 1; i > 0; i--) {
        size_t j = (size_t)rand() % (i + 1);
        const char *temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
    }
    size_t pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = my_strlen(strings[i]);
        for (size_t k = 0; k < len; k++) {
            res[pos++] = strings[i][k];
        }
    }
    res[total_len] = '\0';
    *out = res;
    return STATUS_OK;
}