#include <stdio.h>
#include <stdlib.h>
#include "3lab2pac.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Ошибка: неверное количество аргументов.\n");
        fprintf(stderr, "Использование: %s <подстрока> <файл1> [файл2] ...\n", argv[0]);
        return 1;
    }

    if (argc > 12) {
        fprintf(stderr, "Ошибка: поддерживается не более 10 файлов.\n");
        return 1;
    }

    const char *substring = argv[1];
    size_t file_count = (size_t)(argc - 2);
    Match *matches = NULL;
    size_t match_count = 0;
    status_t st = STATUS_OK;

    switch (file_count) {
        case 1: st = search_in_files(substring, 1, &matches, &match_count, argv[2]); break;
        case 2: st = search_in_files(substring, 2, &matches, &match_count, argv[2], argv[3]); break;
        case 3: st = search_in_files(substring, 3, &matches, &match_count, argv[2], argv[3], argv[4]); break;
        case 4: st = search_in_files(substring, 4, &matches, &match_count, argv[2], argv[3], argv[4], argv[5]); break;
        case 5: st = search_in_files(substring, 5, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6]); break;
        case 6: st = search_in_files(substring, 6, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]); break;
        case 7: st = search_in_files(substring, 7, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]); break;
        case 8: st = search_in_files(substring, 8, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]); break;
        case 9: st = search_in_files(substring, 9, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10]); break;
        case 10: st = search_in_files(substring, 10, &matches, &match_count, argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11]); break;
    }

    if (st != STATUS_OK) {
        if (st == STATUS_ERR_FILE_OPEN) {
            fprintf(stderr, "Ошибка: не удалось открыть один из файлов.\n");
        } else if (st == STATUS_ERR_MEMORY) {
            fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        } else if (st == STATUS_ERR_INVALID_DATA) {
            fprintf(stderr, "Ошибка: некорректные входные данные.\n");
        } else if (st == STATUS_ERR_FILE_READ) {
            fprintf(stderr, "Ошибка: сбой при чтении файла.\n");
        } else {
            fprintf(stderr, "Ошибка выполнения (код %d).\n", st);
        }
        free_matches(matches);
        return 1;
    }

    for (size_t i = 0; i < match_count; i++) {
        printf("Файл: %s, Строка: %zu, Символ: %zu\n", 
               matches[i].filename, matches[i].line, matches[i].char_index);
    }

    free_matches(matches);
    return 0;
}