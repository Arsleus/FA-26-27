#ifndef LAB3_PAC1_H
#define LAB3_PAC1_H

#include <stddef.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_FLAG,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

typedef struct {
    double a;
    double b;
    double c;
    double root1;
    double root2;
    int roots_count;
} EquationResult;

status_t check_multiple(int a, int b, int *out_result);
status_t check_right_triangle(double a, double b, double c, double epsilon, int *out_result);
status_t solve_quadratic_unique(double a, double b, double c, double epsilon, EquationResult **out_results, size_t *out_count);

#endif