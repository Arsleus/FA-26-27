#include "3lab1pac.h"
#include <stdlib.h>
#include <math.h>

status_t check_multiple(int a, int b, int *out_result) {
    if (out_result == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (b == 0) {
        return STATUS_ERR_INVALID_DATA;
    }
    *out_result = (a % b == 0) ? 1 : 0;
    return STATUS_OK;
}

status_t check_right_triangle(double a, double b, double c, double epsilon, int *out_result) {
    if (out_result == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    if (a <= 0.0 || b <= 0.0 || c <= 0.0) {
        return STATUS_ERR_INVALID_DATA;
    }

    double max_side = a;
    double s1 = b;
    double s2 = c;
    
    if (b > max_side) { max_side = b; s1 = a; s2 = c; }
    if (c > max_side) { max_side = c; s1 = a; s2 = b; }

    double sum_sq = s1 * s1 + s2 * s2;
    double max_sq = max_side * max_side;

    if (fabs(sum_sq - max_sq) < epsilon) {
        *out_result = 1;
    } else {
        *out_result = 0;
    }
    return STATUS_OK;
}

static int is_equal_double(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

static int is_permutation_used(const EquationResult *results, size_t count, double a, double b, double c, double epsilon) {
    for (size_t i = 0; i < count; i++) {
        if (is_equal_double(results[i].a, a, epsilon) &&
            is_equal_double(results[i].b, b, epsilon) &&
            is_equal_double(results[i].c, c, epsilon)) {
            return 1;
        }
    }
    return 0;
}

status_t solve_quadratic_unique(double a, double b, double c, double epsilon, EquationResult **out_results, size_t *out_count) {
    if (out_results == NULL || out_count == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    *out_results = NULL;
    *out_count = 0;

    if (epsilon <= 0.0) {
        return STATUS_ERR_INVALID_DATA;
    }

    double coeffs[3] = {a, b, c};
    int perms[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

    EquationResult *results = NULL;
    size_t count = 0;

    for (int i = 0; i < 6; i++) {
        double cur_a = coeffs[perms[i][0]];
        double cur_b = coeffs[perms[i][1]];
        double cur_c = coeffs[perms[i][2]];

        if (is_permutation_used(results, count, cur_a, cur_b, cur_c, epsilon)) {
            continue;
        }

        EquationResult *new_results = (EquationResult *)realloc(results, (count + 1) * sizeof(EquationResult));
        if (new_results == NULL) {
            free(results);
            return STATUS_ERR_MEMORY;
        }
        results = new_results;

        results[count].a = cur_a + 0.0;
        results[count].b = cur_b + 0.0;
        results[count].c = cur_c + 0.0;
        results[count].roots_count = 0;

        if (fabs(cur_a) < epsilon) {
            if (fabs(cur_b) < epsilon) {
                if (fabs(cur_c) < epsilon) {
                    results[count].roots_count = -1;
                } else {
                    results[count].roots_count = 0;
                }
            } else {
                results[count].root1 = -cur_c / cur_b + 0.0;
                results[count].roots_count = 1;
            }
        } else {
            double d = cur_b * cur_b - 4.0 * cur_a * cur_c;
            if (d < -epsilon) {
                results[count].roots_count = 0;
            } else if (fabs(d) <= epsilon) {
                results[count].root1 = -cur_b / (2.0 * cur_a) + 0.0;
                results[count].roots_count = 1;
            } else {
                results[count].root1 = (-cur_b + sqrt(d)) / (2.0 * cur_a) + 0.0;
                results[count].root2 = (-cur_b - sqrt(d)) / (2.0 * cur_a) + 0.0;
                results[count].roots_count = 2;
            }
        }
        count++;
    }

    *out_results = results;
    *out_count = count;
    return STATUS_OK;
}