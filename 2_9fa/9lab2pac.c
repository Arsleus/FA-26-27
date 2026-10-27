#include "9lab2pac.h"
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

static int is_finite_fraction(int base, double fraction, double epsilon) {
    int cap = (int)ceil((double)DBL_MANT_DIG * log(2.0) / log((double)base));
    int significant = 0;

    for (int i = 0; ; i++) {
        fraction *= base;
        fraction -= (int)fraction;

        if (fraction > epsilon) {
            significant++;
        }
        if (fraction <= epsilon) {
            return 1;
        }
        if (significant >= cap) {
            return 0;
        }
    }
}

status_t check_fractions(int base, size_t count, double epsilon, int **out_results, ...) {
    if (out_results == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    *out_results = NULL;

    if (base < 2 || base > 36 || count == 0 || epsilon <= 0.0) {
        return STATUS_ERR_INVALID_DATA;
    }

    if (count > SIZE_MAX / sizeof(int)) {
        return STATUS_ERR_OVERFLOW;
    }

    *out_results = (int *)malloc(count * sizeof(int));
    if (*out_results == NULL) {
        return STATUS_ERR_MEMORY;
    }

    va_list args;
    va_start(args, out_results);

    for (size_t i = 0; i < count; i++) {
        double fraction = va_arg(args, double);

        if (fraction <= 0.0 || fraction >= 1.0) {
            free(*out_results);
            *out_results = NULL;
            va_end(args);
            return STATUS_ERR_INVALID_DATA;
        }

        (*out_results)[i] = is_finite_fraction(base, fraction, epsilon);
    }

    va_end(args);
    return STATUS_OK;
}

void free_results(int *results) {
    if (results != NULL) {
        free(results);
    }
}