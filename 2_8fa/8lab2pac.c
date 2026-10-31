#include "8lab2pac.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

static int char_to_val(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 10;
    }
    return -1;
}

static char val_to_char(int val) {
    if (val >= 0 && val <= 9) {
        return (char)('0' + val);
    }
    if (val >= 10 && val <= 35) {
        return (char)('A' + (val - 10));
    }
    return '?';
}

static int is_valid_number(const char *s, int base) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    }
    for (size_t i = 0; s[i] != '\0'; i++) {
        int val = char_to_val(s[i]);
        if (val < 0 || val >= base) {
            return 0;
        }
    }
    return 1;
}

static status_t add_in_base(const char *a, const char *b, int base, char **out_res) {
    if (a == NULL || b == NULL || out_res == NULL) {
        return STATUS_ERR_NULL_ARG;
    }

    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t max_len = (len_a > len_b) ? len_a : len_b;

    if (max_len > SIZE_MAX - 2) {
        return STATUS_ERR_OVERFLOW;
    }

    char *res = (char *)malloc(max_len + 2);
    if (res == NULL) {
        return STATUS_ERR_MEMORY;
    }

    size_t i = len_a;
    size_t j = len_b;
    size_t k = 0;
    int carry = 0;

    while (i > 0 || j > 0 || carry > 0) {
        int sum = carry;
        
        if (i > 0) {
            sum += char_to_val(a[i - 1]);
            i--;
        }
        if (j > 0) {
            sum += char_to_val(b[j - 1]);
            j--;
        }

        res[k++] = val_to_char(sum % base);
        carry = sum / base;
    }
    res[k] = '\0';

    for (size_t left = 0, right = k - 1; left < right; left++, right--) {
        char temp = res[left];
        res[left] = res[right];
        res[right] = temp;
    }

    char *start = res;
    while (*start == '0' && *(start + 1) != '\0') {
        start++;
    }

    if (start != res) {
        memmove(res, start, strlen(start) + 1);
    }

    *out_res = res;
    return STATUS_OK;
}

status_t sum_strings_in_base(int base, size_t count, char **out_sum, ...) {
    if (out_sum == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    *out_sum = NULL;

    if (base < 2 || base > 36 || count == 0) {
        return STATUS_ERR_INVALID_DATA;
    }

    char *current_sum = (char *)malloc(2);
    if (current_sum == NULL) {
        return STATUS_ERR_MEMORY;
    }
    current_sum[0] = '0';
    current_sum[1] = '\0';

    va_list args;
    va_start(args, out_sum);

    for (size_t i = 0; i < count; i++) {
        const char *next_str = va_arg(args, const char *);
        
        if (next_str == NULL || !is_valid_number(next_str, base)) {
            free(current_sum);
            va_end(args);
            return STATUS_ERR_INVALID_DATA;
        }

        char *new_sum = NULL;
        status_t st = add_in_base(current_sum, next_str, base, &new_sum);
        
        free(current_sum);

        if (st != STATUS_OK) {
            va_end(args);
            return st;
        }
        current_sum = new_sum;
    }

    va_end(args);
    *out_sum = current_sum;
    return STATUS_OK;
}

void free_sum(char *sum) {
    if (sum != NULL) {
        free(sum);
    }
}