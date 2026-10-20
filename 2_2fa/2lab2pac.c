#include "2lab2pac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>

static int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int is_valid_name(const char *s) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    }
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (!is_letter(s[i])) {
            return 0;
        }
    }
    return 1;
}

static status_t parse_id(const char *s, unsigned int *out) {
    if (s == NULL || out == NULL || s[0] == '\0') {
        return STATUS_ERR_INVALID_DATA;
    }
    unsigned int result = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return STATUS_ERR_INVALID_DATA;
        }
        unsigned int digit = (unsigned int)(s[i] - '0');
        if (result > (UINT_MAX - digit) / 10) {
            return STATUS_ERR_INVALID_DATA;
        }
        result = result * 10 + digit;
    }
    *out = result;
    return STATUS_OK;
}

void free_employees(Employee *arr, size_t count) {
    if (arr == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(arr[i].name);
        free(arr[i].surname);
    }
    free(arr);
}

static int compare_double(double a, double b, double epsilon) {
    if (a - b > epsilon) {
        return 1;
    }
    if (b - a > epsilon) {
        return -1;
    }
    return 0;
}

static int cmp_asc(const void *a, const void *b) {
    const Employee *e1 = (const Employee *)a;
    const Employee *e2 = (const Employee *)b;

    int cmp_sal = compare_double(e1->salary, e2->salary, EPSILON);
    if (cmp_sal != 0) {
        return cmp_sal;
    }

    int cmp_sur = strcmp(e1->surname, e2->surname);
    if (cmp_sur != 0) {
        return cmp_sur;
    }

    int cmp_nam = strcmp(e1->name, e2->name);
    if (cmp_nam != 0) {
        return cmp_nam;
    }

    if (e1->id < e2->id) return -1;
    if (e1->id > e2->id) return 1;
    return 0;
}

static int cmp_desc(const void *a, const void *b) {
    return -cmp_asc(a, b);
}

status_t sort_employees(Employee *arr, size_t count, int is_descending) {
    if (arr == NULL && count > 0) {
        return STATUS_ERR_NULL_ARG;
    }
    if (count == 0) {
        return STATUS_OK;
    }

    if (is_descending) {
        qsort(arr, count, sizeof(Employee), cmp_desc);
    } else {
        qsort(arr, count, sizeof(Employee), cmp_asc);
    }

    return STATUS_OK;
}

status_t read_employees(const char *filename, Employee **out_arr, size_t *out_count) {
    if (filename == NULL || out_arr == NULL || out_count == NULL) {
        return STATUS_ERR_NULL_ARG;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return STATUS_ERR_FILE_OPEN;
    }

    size_t count = 0;
    size_t max_len = 0;
    size_t current_len = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            if (current_len > max_len) {
                max_len = current_len;
            }
            current_len = 0;
            count++;
        } else {
            current_len++;
        }
    }
    
    if (current_len > 0) {
        if (current_len > max_len) {
            max_len = current_len;
        }
        count++;
    }

    if (ferror(f)) {
        fclose(f);
        return STATUS_ERR_FILE_READ;
    }

    if (count == 0) {
        fclose(f);
        return STATUS_ERR_EMPTY_FILE;
    }

    if (count > SIZE_MAX / sizeof(Employee) || max_len > SIZE_MAX - 2) {
        fclose(f);
        return STATUS_ERR_OVERFLOW;
    }

    Employee *arr = malloc(count * sizeof(Employee));
    if (arr == NULL) {
        fclose(f);
        return STATUS_ERR_MEMORY;
    }

    char *buffer = malloc(max_len + 2);
    if (buffer == NULL) {
        free(arr);
        fclose(f);
        return STATUS_ERR_MEMORY;
    }

    rewind(f);

    for (size_t i = 0; i < count; i++) {
        if (fgets(buffer, (int)(max_len + 2), f) == NULL) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_FILE_READ;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        char *id_str = strtok(buffer, " \t\r\n");
        char *name_str = strtok(NULL, " \t\r\n");
        char *surname_str = strtok(NULL, " \t\r\n");
        char *salary_str = strtok(NULL, " \t\r\n");

        if (id_str == NULL || name_str == NULL || surname_str == NULL || salary_str == NULL) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_INVALID_DATA;
        }

        if (strtok(NULL, " \t\r\n") != NULL) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_INVALID_DATA;
        }

        unsigned int id_val = 0;
        if (parse_id(id_str, &id_val) != STATUS_OK) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_INVALID_DATA;
        }

        if (!is_valid_name(name_str) || !is_valid_name(surname_str)) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_INVALID_DATA;
        }

        char *endptr;
        double salary_val = strtod(salary_str, &endptr);
        if (*endptr != '\0' || salary_val < 0.0 || isinf(salary_val)) {
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_INVALID_DATA;
        }

        arr[i].name = malloc(strlen(name_str) + 1);
        arr[i].surname = malloc(strlen(surname_str) + 1);
        if (arr[i].name == NULL || arr[i].surname == NULL) {
            free(arr[i].name);
            free(arr[i].surname);
            free(buffer);
            free_employees(arr, i);
            fclose(f);
            return STATUS_ERR_MEMORY;
        }
        strcpy(arr[i].name, name_str);
        strcpy(arr[i].surname, surname_str);

        arr[i].id = id_val;
        arr[i].salary = salary_val;
    }

    free(buffer);
    fclose(f);
    
    *out_arr = arr;
    *out_count = count;
    
    return STATUS_OK;
}

status_t write_employees(const char *filename, const Employee *arr, size_t count) {
    if (filename == NULL || (arr == NULL && count > 0)) {
        return STATUS_ERR_NULL_ARG;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return STATUS_ERR_FILE_OPEN;
    }

    for (size_t i = 0; i < count; i++) {
        if (fprintf(f, "%u %s %s %g\n", arr[i].id, arr[i].name, arr[i].surname, arr[i].salary) < 0) {
            fclose(f);
            return STATUS_ERR_FILE_WRITE;
        }
    }

    if (ferror(f)) {
        fclose(f);
        return STATUS_ERR_FILE_WRITE;
    }

    if (fclose(f) != 0) {
        return STATUS_ERR_FILE_WRITE;
    }

    return STATUS_OK;
}