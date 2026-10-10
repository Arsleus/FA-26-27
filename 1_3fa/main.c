#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "3lab1pac.h"

static int parse_double(const char *s, double *out) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    }
    
    char *endptr;
    double val = strtod(s, &endptr);
    
    if (*endptr != '\0' || isinf(val)) {
        return 0;
    }
    *out = val;
    return 1;
}

static int parse_int(const char *s, int *out) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    }
    
    int sign = 1;
    size_t i = 0;
    
    if (s[0] == '-') {
        sign = -1;
        i++;
    }
    
    if (s[i] == '\0') {
        return 0;
    }
    
    int result = 0;
    
    for (; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
        
        int digit = s[i] - '0';
        
        if (sign == 1) {
            if (result > (INT_MAX - digit) / 10) {
                return 0;
            }
        } else {
            if (result < (INT_MIN + digit) / 10) {
                return 0;
            }
        }
        
        result = result * 10 + sign * digit;
    }
    
    *out = result;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Ошибка: неверное количество аргументов\n");
        return 1;
    }

    const char *flag = argv[1];
    if (flag[0] != '-' && flag[0] != '/') {
        fprintf(stderr, "Ошибка: флаг должен начинаться с '-' или '/'\n");
        return 1;
    }

    char action = flag[1];
    if (flag[2] != '\0') {
        fprintf(stderr, "Ошибка: неверный формат флага\n");
        return 1;
    }

    status_t st = STATUS_OK;

    if (action == 'm') {
        if (argc != 4) {
            fprintf(stderr, "Ошибка: для флага -m нужно ровно 2 числа\n");
            return 1;
        }

        int a = 0;
        int b = 0;
        if (!parse_int(argv[2], &a) || !parse_int(argv[3], &b)) {
            fprintf(stderr, "Ошибка: введены некорректные целые числа\n");
            return 1;
        }

        int result = 0;
        st = check_multiple(a, b, &result);

        if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: второе число не может быть нулем\n");
            return 1;
        } else if (st != STATUS_OK) {
            fprintf(stderr, "Ошибка выполнения функции (код %d)\n", st);
            return 1;
        }

        if (result == 1) {
            printf("Число %d кратно числу %d\n", a, b);
        } else {
            printf("Число %d не кратно числу %d\n", a, b);
        }
    } else if (action == 't') {
        if (argc != 6) {
            fprintf(stderr, "Ошибка: для флага -t нужен эпсилон и 3 стороны\n");
            return 1;
        }

        double epsilon = 0.0;
        double a = 0.0, b = 0.0, c = 0.0;

        if (!parse_double(argv[2], &epsilon) || 
            !parse_double(argv[3], &a) || 
            !parse_double(argv[4], &b) || 
            !parse_double(argv[5], &c)) {
            fprintf(stderr, "Ошибка: введены некорректные вещественные числа\n");
            return 1;
        }

        if (epsilon <= 0.0) {
            fprintf(stderr, "Ошибка: точность (эпсилон) должна быть больше нуля\n");
            return 1;
        }

        int is_right = 0;
        st = check_right_triangle(a, b, c, epsilon, &is_right);

        if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: стороны треугольника должны быть больше нуля\n");
            return 1;
        } else if (st != STATUS_OK) {
            fprintf(stderr, "Ошибка выполнения функции (код %d)\n", st);
            return 1;
        }

        if (is_right) {
            printf("Треугольник со сторонами %g, %g, %g является прямоугольным\n", a, b, c);
        } else {
            printf("Треугольник со сторонами %g, %g, %g не является прямоугольным\n", a, b, c);
        }
    } else if (action == 'q') {
        if (argc != 6) {
            fprintf(stderr, "Ошибка: для флага -q нужен эпсилон и 3 коэффициента\n");
            return 1;
        }

        double epsilon = 0.0;
        double a = 0.0, b = 0.0, c = 0.0;

        if (!parse_double(argv[2], &epsilon) || 
            !parse_double(argv[3], &a) || 
            !parse_double(argv[4], &b) || 
            !parse_double(argv[5], &c)) {
            fprintf(stderr, "Ошибка: введены некорректные вещественные числа\n");
            return 1;
        }

        if (epsilon <= 0.0) {
            fprintf(stderr, "Ошибка: точность (эпсилон) должна быть больше нуля\n");
            return 1;
        }

        EquationResult *results = NULL;
        size_t count = 0;
        
        st = solve_quadratic_unique(a, b, c, epsilon, &results, &count);

        if (st != STATUS_OK) {
            fprintf(stderr, "Ошибка выполнения функции (код %d)\n", st);
            return 1;
        }

        for (size_t i = 0; i < count; i++) {
            printf("Уравнение: %gx^2 + %gx + %g = 0\n", results[i].a, results[i].b, results[i].c);
            
            if (results[i].roots_count == 0) {
                printf("  Корней нет\n");
            } else if (results[i].roots_count == -1) {
                printf("  Бесконечно много корней\n");
            } else if (results[i].roots_count == 1) {
                printf("  Один корень: x = %g\n", results[i].root1);
            } else if (results[i].roots_count == 2) {
                printf("  Два корня: x1 = %g, x2 = %g\n", results[i].root1, results[i].root2);
            }
        }

        free(results);
    }
    else {
        fprintf(stderr, "Ошибка: неизвестный флаг\n");
        return 1;
    }

    return 0;
}