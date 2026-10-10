#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "9lab1pac.h"

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
            if (result > (INT_MAX - digit) / 10) return 0;
        } else {
            if (result < (INT_MIN + digit) / 10) return 0;
        }
        result = result * 10 + sign * digit;
    }
    
    *out = result;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Ошибка: неверное количество аргументов\n");
        fprintf(stderr, "%s <a> <b>\n", argv[0]);
        return 1;
    }

    int a = 0, b = 0;
    if (!parse_int(argv[1], &a) || !parse_int(argv[2], &b)) {
        fprintf(stderr, "Ошибка: Некорректные числа\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    int fixed_arr[FIXED_SIZE];
    int min_val = 0, max_val = 0;

    int temp_a = a;
    int temp_b = b;
    if (temp_a > temp_b) {
        int temp = temp_a;
        temp_a = temp_b;
        temp_b = temp;
    }

    for (size_t i = 0; i < FIXED_SIZE; i++) {
        fixed_arr[i] = rand() % (temp_b - temp_a + 1) + temp_a;
    }

    for (size_t i = 0; i < FIXED_SIZE; i++) {
        printf("%d ", fixed_arr[i]);
    }
    printf("\n");
    
    status_t st = process_fixed_array(fixed_arr, FIXED_SIZE, &min_val, &max_val);
    if (st != STATUS_OK) {
        fprintf(stderr, "Ошибка выполнения (код %d).\n", st);
        return 1;
    }

    for (size_t i = 0; i < FIXED_SIZE; i++) {
        printf("%d ", fixed_arr[i]);
    }
    printf("\n");

    size_t size_c = 0;
    int *array_c = NULL;

    st = process_dynamic_arrays(&size_c, &array_c);
    if (st != STATUS_OK) {
        if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        } else {
            fprintf(stderr, "Ошибка выполнения (код %d).\n", st);
        }
        return 1;
    }
    
    for (size_t i = 0; i < size_c; i++) {
        printf("%d ", array_c[i]);
    }

    printf("\n");

    free_dynamic_array(array_c);

    return 0;
}