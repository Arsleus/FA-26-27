#include "1lab3pac.h"
#include <stdlib.h>
#include <limits.h>

static unsigned int bitwise_add(unsigned int a, unsigned int b) {
    while (b != 0) {
        unsigned int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

static unsigned int bitwise_sub(unsigned int a, unsigned int b) {
    unsigned int neg_b = bitwise_add(~b, 1);
    return bitwise_add(a, neg_b);
}

static unsigned int parse_string_to_uint(const char *s, int *is_valid, int *is_negative) {
    *is_valid = 1;
    *is_negative = 0;
    unsigned int result = 0;
    unsigned int i = 0;

    if (s == NULL || s[0] == '\0') {
        *is_valid = 0;
        return 0;
    }

    if (s[0] == '-') {
        *is_negative = 1;
        i = bitwise_add(i, 1);
    }

    if (s[i] == '\0') {
        *is_valid = 0;
        return 0;
    }

    while (s[i] != '\0') {
        if (s[i] < '0' || s[i] > '9') {
            *is_valid = 0;
            return 0;
        }

        unsigned int digit = bitwise_sub((unsigned int)s[i], (unsigned int)'0');

        if (result > (UINT_MAX >> 3)) {
            *is_valid = 0;
            return 0;
        }

        unsigned int mul_8 = result << 3;
        unsigned int mul_2 = result << 1;

        if (mul_8 > bitwise_sub(UINT_MAX, mul_2)) {
            *is_valid = 0;
            return 0;
        }

        unsigned int mul_10 = bitwise_add(mul_8, mul_2);

        if (mul_10 > bitwise_sub(UINT_MAX, digit)) {
            *is_valid = 0;
            return 0;
        }
        result = bitwise_add(mul_10, digit);

        i = bitwise_add(i, 1);
    }
    return result;
}

status_t convert_to_base_2r(const char *num_str, int r, char **out_str) {
    if (num_str == NULL || out_str == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    *out_str = NULL;

    if (r < 1 || r > 5) {
        return STATUS_ERR_INVALID_DATA;
    }

    int is_valid = 0;
    int is_negative = 0;
    unsigned int val = parse_string_to_uint(num_str, &is_valid, &is_negative);
    if (!is_valid) {
        return STATUS_ERR_INVALID_DATA;
    }

    if (val == 0) {
        *out_str = (char *)malloc(2);
        if (*out_str == NULL) return STATUS_ERR_MEMORY;
        (*out_str)[0] = '0';
        (*out_str)[1] = '\0';
        return STATUS_OK;
    }

    char buf[33];
    int pos = 32;
    buf[pos] = '\0';

    unsigned int base = (unsigned int)(1 << r);
    unsigned int mask = bitwise_sub(base, 1);

    while (val != 0) {
        unsigned int digit = val & mask;
        char c;
        
        if (digit < 10) {
            c = (char)bitwise_add((unsigned int)'0', digit);
        } else {
            c = (char)bitwise_add((unsigned int)'A', bitwise_sub(digit, 10));
        }
        
        pos = (int)bitwise_sub((unsigned int)pos, 1);
        buf[pos] = c;
        
        val = val >> r;
    }

    unsigned int len = bitwise_sub(32, (unsigned int)pos);
    unsigned int alloc_size = bitwise_add(len, 1);
    
    if (is_negative) {
        alloc_size = bitwise_add(alloc_size, 1);
    }

    *out_str = (char *)malloc(alloc_size);
    if (*out_str == NULL) {
        return STATUS_ERR_MEMORY;
    }

    unsigned int start_idx = 0;
    if (is_negative) {
        (*out_str)[0] = '-';
        start_idx = 1;
    }

    unsigned int i = 0;
    while (i < len) {
        (*out_str)[bitwise_add(start_idx, i)] = buf[bitwise_add((unsigned int)pos, i)];
        i = bitwise_add(i, 1);
    }
    (*out_str)[bitwise_add(start_idx, len)] = '\0';

    return STATUS_OK;
}