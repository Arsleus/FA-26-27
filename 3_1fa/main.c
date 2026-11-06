#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "1lab3pac.h"

static int parse_r(const char *s, int *out_r) {
    if (s == NULL || s[0] == '\0' || s[1] != '\0') {
        return 0;
    }
    if (s[0] < '1' || s[0] > '5') {
        return 0;
    }
    *out_r = s[0] - '0';
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Неверное количество аргументов\n");
        fprintf(stderr, "%s <число> <r>\n", argv[0]);
        return 1;
    }

    int r = 0;
    if (!parse_r(argv[2], &r)) {
        fprintf(stderr, "Ошибка: параметр r должен быть целым числом от 1 до 5\n");
        return 1;
    }

    char *result = NULL;
    status_t st = convert_to_base_2r(argv[1], r, &result);

    if (st != STATUS_OK) {
        if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: некорректное число\n");
        } else if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти\n");
        } else {
            fprintf(stderr, "Ошибка выполнения (код %d)\n", st);
        }
        return 1;
    }

    printf("Число в системе с основанием 2^%d: %s\n", r, result);
    
    free(result);

    return 0;
}