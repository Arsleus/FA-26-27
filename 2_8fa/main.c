#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "8lab2pac.h"

static int parse_base(const char *s, int *out_base) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    }
    
    int result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
        int digit = s[i] - '0';
        result = result * 10 + digit;
        if (result > 36) {
            return 0;
        }
    }
    
    if (result < 2) {
        return 0;
    }
    *out_base = result;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Ошибка: неверное количество аргументов\n");
        fprintf(stderr, "%s <основание> <число1> [число2] ...\n", argv[0]);
        return 1;
    }
    
    if (argc > 12) {
        fprintf(stderr, "Ошибка: поддерживается не более 10 чисел\n");
        return 1;
    }

    int base = 0;
    if (!parse_base(argv[1], &base)) {
        fprintf(stderr, "Ошибка: основание должно быть целым числом от 2 до 36\n");
        return 1;
    }

    size_t count = (size_t)(argc - 2);
    char *result = NULL;
    status_t st = STATUS_OK;

    switch (count) {
        case 1: st = sum_strings_in_base(base, 1, &result, argv[2]); break;
        case 2: st = sum_strings_in_base(base, 2, &result, argv[2], argv[3]); break;
        case 3: st = sum_strings_in_base(base, 3, &result, argv[2], argv[3], argv[4]); break;
        case 4: st = sum_strings_in_base(base, 4, &result, argv[2], argv[3], argv[4], argv[5]); break;
        case 5: st = sum_strings_in_base(base, 5, &result, argv[2], argv[3], argv[4], argv[5], argv[6]); break;
        case 6: st = sum_strings_in_base(base, 6, &result, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]); break;
        case 7: st = sum_strings_in_base(base, 7, &result, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]); break;
        case 8: st = sum_strings_in_base(base, 8, &result, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]); break;
        case 9: st = sum_strings_in_base(base, 9, &result, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10]); break;
        case 10: st = sum_strings_in_base(base, 10, &result, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11]); break;
    }

    if (st != STATUS_OK) {
        if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: некорректные данные\n");
        } else if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти\n");
        } else {
            fprintf(stderr, "Ошибка выполнения (код %d)\n", st);
        }
        return 1;
    }

    printf("Сумма в основании %d: %s\n", base, result);
    free_sum(result);

    return 0;
}