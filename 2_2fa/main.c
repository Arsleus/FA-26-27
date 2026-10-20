#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "2lab2pac.h"

static const char *get_filename(const char *path) {
    const char *last_slash = NULL;
    for (size_t i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/' || path[i] == '\\') {
            last_slash = &path[i];
        }
    }
    if (last_slash != NULL) {
        return last_slash + 1;
    }
    return path;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Ошибка: неверное количество аргументов.\n");
        return 1;
    }

    const char *in_file = argv[1];
    const char *flag = argv[2];
    const char *out_file = argv[3];

    if (strcmp(get_filename(in_file), get_filename(out_file)) == 0) {
        fprintf(stderr, "Ошибка: имена входного и выходного файлов совпадают.\n");
        return 1;
    }

    if (flag[0] != '-' && flag[0] != '/') {
        fprintf(stderr, "Ошибка: флаг должен начинаться с '-' или '/'.\n");
        return 1;
    }

    int is_descending = 0;
    if (flag[1] == 'a' && flag[2] == '\0') {
        is_descending = 0;
    } else if (flag[1] == 'd' && flag[2] == '\0') {
        is_descending = 1;
    } else {
        fprintf(stderr, "Ошибка: неверный флаг. Допустимы -a, -d, /a, /d.\n");
        return 1;
    }

    Employee *arr = NULL;
    size_t count = 0;
    status_t st;

    st = read_employees(in_file, &arr, &count);
    if (st != STATUS_OK) {
        if (st == STATUS_ERR_FILE_OPEN) {
            fprintf(stderr, "Ошибка: не удалось открыть входной файл.\n");
        } else if (st == STATUS_ERR_EMPTY_FILE) {
            fprintf(stderr, "Ошибка: входной файл пуст.\n");
        } else if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: некорректные данные во входном файле.\n");
        } else if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        } else {
            fprintf(stderr, "Ошибка при чтении файла (код %d).\n", st);
        }
        return 1;
    }

    st = sort_employees(arr, count, is_descending);
    if (st != STATUS_OK) {
        fprintf(stderr, "Ошибка при сортировке данных.\n");
        free_employees(arr, count);
        return 1;
    }

    st = write_employees(out_file, arr, count);
    free_employees(arr, count);

    if (st != STATUS_OK) {
        if (st == STATUS_ERR_FILE_OPEN) {
            fprintf(stderr, "Ошибка: не удалось открыть выходной файл для записи.\n");
        } else if (st == STATUS_ERR_FILE_WRITE) {
            fprintf(stderr, "Ошибка: не удалось записать данные в файл.\n");
        } else {
            fprintf(stderr, "Ошибка при записи в файл (код %d).\n", st);
        }
        return 1;
    }

    return 0;
}