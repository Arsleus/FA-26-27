#include "lab1fa.h"

FuncStatus ParseLong(const char *str, long *result) {
    if (str == NULL || *str == '\0') {
        return StatusInvalidParameter;
    }
    char *endptr;
    long value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return StatusInvalidParameter;
    }
    if (value > LONG_MAX || value < LONG_MIN) {
        return StatusOverflow;
    }
    *result = value;
    return StatusSuccess;
}

FuncStatus FindMultiplies(long x, int *has_multiplies) {
    if (x <= 0 || x > 100) {
        *has_multiplies = 0;
        return StatusSuccess;
    }
    *has_multiplies = 1;
    return StatusSuccess;
}

FuncStatus CheckPrime(long x, NumberType *result) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    if (x == 1) {
        *result = TypeNeither;
        return StatusSuccess;
    }
    if (x == 2) {
        *result = TypePrime;
        return StatusSuccess;
    }
    if (x % 2 == 0) {
        *result = TypeComposite;
        return StatusSuccess;
    }
    for (long i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            *result = TypeComposite;
            return StatusSuccess;
        }
    }
    *result = TypePrime;
    return StatusSuccess;
}

FuncStatus SplitToHexDigits(long x, int *digits_buffer, int *count) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    int local_count = 0;
    while (x > 0) {
        digits_buffer[local_count] = x % 16;
        x /= 16;
        local_count++;
    }
    *count = local_count;
    return StatusSuccess;
}

FuncStatus CreatePowersTable(int x, long long *table) {
    if (x <= 0 || x > 10) {
        return StatusInvalidParameter;
    }
    for (int base = 1; base <= 10; ++base) {
        long long current_power = 1;
        for (int exp = 1; exp <= x; ++exp) {
            current_power *= base;
            table[(base - 1) * x + (exp - 1)] = current_power;
        }
    }
    return StatusSuccess;
}

FuncStatus CalculateNaturalSum(long x, long long *sum) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    if ((long long)x > LLONG_MAX / (x + 1)) {
        return StatusOverflow;
    }
    *sum = ((long long)x * (x + 1)) / 2;
    return StatusSuccess;
}

FuncStatus CalculateFactorial(int x, long long *factorial) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    long long current_factorial = 1;
    for (int i = 1; i <= x; ++i) {
        if (current_factorial > LLONG_MAX / i) {
            return StatusOverflow;
        }
        current_factorial *= i;
    }
    *factorial = current_factorial;
    return StatusSuccess;
}
