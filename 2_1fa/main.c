#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "1lab2pac.h"

static int is_valid_flag(const char *flag) {
    if (flag == NULL || flag[0] != '-' || flag[1] == '\0' || flag[2] != '\0') {
        return 0;
    }
    if (flag[1] == 'l' || flag[1] == 'r' || flag[1] == 'u' || flag[1] == 'n' || flag[1] == 'c') {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    status_t st = STATUS_OK;

    if (argc < 3) {
        st = STATUS_ERR_ARGC;
    } else {
        const char *flag = argv[1];
        const char *str = argv[2];

        if (!is_valid_flag(flag)) {
            st = STATUS_ERR_FLAG;
        } else if (flag[1] != 'c' && argc != 3) {
            st = STATUS_ERR_ARGC;
        } else {
            switch (flag[1]) {
                case 'l': {
                    size_t len = 0;
                    st = op_length(str, &len);
                    if (st == STATUS_OK) {
                        printf("%zu\n", len);
                    }
                    break;
                }
                case 'r': {
                    char *res = NULL;
                    st = op_reverse(str, &res);
                    if (st == STATUS_OK) {
                        printf("%s\n", res);
                        free(res);
                    }
                    break;
                }
                case 'u': {
                    char *res = NULL;
                    st = op_upper_odd(str, &res);
                    if (st == STATUS_OK) {
                        printf("%s\n", res);
                        free(res);
                    }
                    break;
                }
                case 'n': {
                    char *res = NULL;
                    st = op_partition(str, &res);
                    if (st == STATUS_OK) {
                        printf("%s\n", res);
                        free(res);
                    }
                    break;
                }
                case 'c': {
                    if (argc < 4) {
                        st = STATUS_ERR_ARGC;
                        break;
                    }
                    unsigned int seed = 0;
                    st = parse_uint(argv[3], &seed);
                    if (st != STATUS_OK) {
                        break;
                    }
                    size_t count = (size_t)(argc - 3);
                    if (count > SIZE_MAX / sizeof(char*)) {
                        st = STATUS_ERR_OVERFLOW;
                        break;
                    }
                    const char **strings = (const char **)malloc(count * sizeof(char*));
                    if (strings == NULL) {
                        st = STATUS_ERR_MEMORY;
                        break;
                    }
                    strings[0] = argv[2];
                    size_t idx = 1;
                    for (int i = 4; i < argc; i++) {
                        strings[idx++] = argv[i];
                    }
                    char *res = NULL;
                    st = op_shuffle_concat(strings, count, seed, &res);
                    if (st == STATUS_OK) {
                        printf("%s\n", res);
                        free(res);
                    }
                    free(strings);
                    break;
                }
            }
        }
    }

    switch (st) {
        case STATUS_OK:
            return 0;
        case STATUS_ERR_ARGC:
            fprintf(stderr, "Ошибка: неверное количество аргументов командной строки.\n");
            break;
        case STATUS_ERR_FLAG:
            fprintf(stderr, "Ошибка: неверный флаг. Допустимы: -l, -r, -u, -n, -c.\n");
            break;
        case STATUS_ERR_EMPTY_STRING:
            fprintf(stderr, "Ошибка: переданная строка не может быть пустой.\n");
            break;
        case STATUS_ERR_MEMORY:
            fprintf(stderr, "Ошибка: недостаточно оперативной памяти.\n");
            break;
        case STATUS_ERR_NULL_ARG:
            fprintf(stderr, "Ошибка: передан нулевой указатель (NULL).\n");
            break;
        case STATUS_ERR_OVERFLOW:
            fprintf(stderr, "Ошибка: переполнение размера данных.\n");
            break;
        case STATUS_ERR_SEED:
            fprintf(stderr, "Ошибка: seed должен быть неотрицательным целым числом.\n");
            break;
        default:
            fprintf(stderr, "Ошибка: неизвестный код ошибки.\n");
            break;
    }

    return 1;
}