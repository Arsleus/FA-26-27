#include <stdio.h>
#include <stdlib.h>
#include "lab1_8.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Ошибка: Неверное количество аргументов.\n");
        printf("Использование: %s <входной_файл> <выходной_файл>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("Ошибка: Не удалось открыть входной файл '%s'.\n", argv[1]);
        return 1;
    }

    FILE *out = fopen(argv[2], "w");
    if (!out) {
        printf("Ошибка: Не удалось создать выходной файл '%s'.\n", argv[2]);
        fclose(in);
        return 1;
    }

    char *num_buf = NULL;
    int buf_capacity = 0; 
    int buf_idx = 0;    
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (buf_idx > 0) {
                char *tmp = realloc(num_buf, buf_idx + 1);
                if (!tmp) {
                    printf("Ошибка: Критическая ошибка выделения памяти.\n");
                    free(num_buf);
                    fclose(in);
                    fclose(out);
                    return 1;
                }
                num_buf = tmp;
                
                num_buf[buf_idx] = '\0';

                const char *start_ptr = num_buf;
                while (*start_ptr == '0') start_ptr++;
                if (*start_ptr == '\0') start_ptr = "0";

                int min_base = 0;
                long long val_dec = 0;

                ParseStatus status = ProcessNumberString(start_ptr, &min_base, &val_dec);

                if (status == StatusSuccess) {
                    fprintf(out, "%s %d %lld\n", start_ptr, min_base, val_dec);
                } else if (status == StatusInvalidCharacter) {
                    fprintf(out, "%s [Ошибка: Некорректный символ]\n", start_ptr);
                } else if (status == StatusOverflow) {
                    fprintf(out, "%s [Ошибка: Переполнение памяти]\n", start_ptr);
                }

                free(num_buf);
                num_buf = NULL;
                buf_capacity = 0;
                buf_idx = 0;
            }
        } else {
            if (buf_idx >= buf_capacity) {
                buf_capacity = buf_idx + 1;
                char *tmp = realloc(num_buf, buf_capacity);
                if (!tmp) {
                    printf("Ошибка: Критическая ошибка выделения памяти.\n");
                    free(num_buf);
                    fclose(in);
                    fclose(out);
                    return 1;
                }
                num_buf = tmp;
            }
            num_buf[buf_idx++] = (char)c;
        }
    }

    if (buf_idx > 0) {
        char *tmp = realloc(num_buf, buf_idx + 1);
        if (tmp) {
            num_buf = tmp;
            num_buf[buf_idx] = '\0';
            const char *start_ptr = num_buf;
            while (*start_ptr == '0') start_ptr++;
            if (*start_ptr == '\0') start_ptr = "0";

            int min_base = 0;
            long long val_dec = 0;
            ParseStatus status = ProcessNumberString(start_ptr, &min_base, &val_dec);

            if (status == StatusSuccess) {
                fprintf(out, "%s %d %lld\n", start_ptr, min_base, val_dec);
            } else if (status == StatusInvalidCharacter) {
                fprintf(out, "%s [Ошибка: Некорректный символ]\n", start_ptr);
            } else if (status == StatusOverflow) {
                fprintf(out, "%s [Ошибка: Переполнение памяти]\n", start_ptr);
            }
        }
        free(num_buf);
    }

    fclose(in);
    fclose(out);
    return 0;
}
