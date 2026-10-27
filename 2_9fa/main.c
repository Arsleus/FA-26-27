#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "9lab2pac.h"

#define EPSILON 1e-9

static int parse_base(const char *s, int *out_base) {
    if (s == NULL || s[0] == '\0') return 0;
    
    int result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
        int digit = s[i] - '0';
        if (result > (36 - digit) / 10) return 0; 
        result = result * 10 + digit;
    }
    
    if (result < 2 || result > 36) return 0;
    *out_base = result;
    return 1;
}

static int parse_fraction(const char *s, double *out_frac) {
    if (s == NULL || s[0] == '\0') return 0;
    if (s[0] == '-' || s[0] == '+' || s[0] == ' ') return 0;
    
    size_t len = strlen(s);
    int significant_digits = 0;
    int point_found = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (s[i] == '.') {
            if (point_found) return 0;
            point_found = 1;
            continue;
        }
        if (s[i] < '0' || s[i] > '9') return 0;
        
        if (s[i] == '0' && significant_digits == 0) {
            continue;
        }
        significant_digits++;
    }

    if (significant_digits > DBL_DIG) {
        return 0;
    }
    
    char *endptr;
    double val = strtod(s, &endptr);
    
    if (*endptr != '\0' || val <= 0.0 || val >= 1.0) {
        return 0;
    }
    *out_frac = val;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Ошибка: неверное количество аргументов.\n");
        fprintf(stderr, "Использование: %s <основание> <дробь1> [дробь2] ...\n", argv[0]);
        return 1;
    }

    int base = 0;
    if (!parse_base(argv[1], &base)) {
        fprintf(stderr, "Ошибка: основание должно быть целым числом от 2 до 36.\n");
        return 1;
    }

    size_t count = (size_t)(argc - 2);
    if (count > 10) {
        fprintf(stderr, "Ошибка: поддерживается не более 10 дробей.\n");
        return 1;
    }

    double *fractions = (double *)malloc(count * sizeof(double));
    if (fractions == NULL) {
        fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        if (!parse_fraction(argv[i + 2], &fractions[i])) {
            fprintf(stderr, "Ошибка: дробь '%s' должна быть вещественным числом в диапазоне (0; 1).\n", argv[i + 2]);
            free(fractions);
            return 1;
        }
    }

    int *results = NULL;
    status_t st = STATUS_OK;

    switch (count) {
        case 1: st = check_fractions(base, 1, EPSILON, &results, fractions[0]); break;
        case 2: st = check_fractions(base, 2, EPSILON, &results, fractions[0], fractions[1]); break;
        case 3: st = check_fractions(base, 3, EPSILON, &results, fractions[0], fractions[1], fractions[2]); break;
        case 4: st = check_fractions(base, 4, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3]); break;
        case 5: st = check_fractions(base, 5, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4]); break;
        case 6: st = check_fractions(base, 6, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4], fractions[5]); break;
        case 7: st = check_fractions(base, 7, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4], fractions[5], fractions[6]); break;
        case 8: st = check_fractions(base, 8, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4], fractions[5], fractions[6], fractions[7]); break;
        case 9: st = check_fractions(base, 9, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4], fractions[5], fractions[6], fractions[7], fractions[8]); break;
        case 10: st = check_fractions(base, 10, EPSILON, &results, fractions[0], fractions[1], fractions[2], fractions[3], fractions[4], fractions[5], fractions[6], fractions[7], fractions[8], fractions[9]); break;
    }

    free(fractions);

    if (st != STATUS_OK) {
        fprintf(stderr, "Ошибка выполнения функции (код %d).\n", st);
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        printf("Дробь %s в основании %d -> %s\n", 
               argv[i + 2], base, 
               results[i] == 1 ? "Конечная" : "Бесконечная");
    }

    free_results(results);
    return 0;
}