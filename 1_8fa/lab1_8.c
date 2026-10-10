#include "lab1_8.h"

int GetSymbolValue(char c) {
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

ParseStatus ProcessNumberString(const char *str, int *min_base, long long *val_dec) {
    if (str == NULL || *str == '\0') {
        return StatusInvalidCharacter;
    }

    int max_digit_val = 0;
    const char *p = str;

    while (*p != '\0') {
        int val = GetSymbolValue(*p);
        if (val == -1) {
            return StatusInvalidCharacter;
        }
        if (val > max_digit_val) {
            max_digit_val = val;
        }
        p++;
    }

    int base = max_digit_val + 1;
    if (base < 2) {
        base = 2;
    }
    *min_base = base;

    long long result = 0;
    p = str;

    while (*p != '\0') {
        int digit = GetSymbolValue(*p);

        if (result > LLONG_MAX / base) {
            return StatusOverflow;
        }
        result *= base;

        if (result > LLONG_MAX - digit) {
            return StatusOverflow;
        }
        result += digit;

        p++;
    }

    *val_dec = result;
    return StatusSuccess;
}
