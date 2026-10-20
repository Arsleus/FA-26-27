#ifndef LAB2PAC_H
#define LAB2PAC_H
#include <stddef.h>
#define EPSILON 1e-6

typedef struct {
    unsigned int id;
    char *name;
    char *surname;
    double salary;
} Employee;

typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_ARGC,
    STATUS_ERR_FLAG,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NULL_ARG,
    STATUS_ERR_OVERFLOW,
    STATUS_ERR_FILE_OPEN,
    STATUS_ERR_FILE_READ,
    STATUS_ERR_FILE_WRITE,
    STATUS_ERR_EMPTY_FILE,
    STATUS_ERR_INVALID_DATA,
    STATUS_ERR_INTERNAL
} status_t;

status_t read_employees(const char *filename, Employee **out_arr, size_t *out_count);
status_t sort_employees(Employee *arr, size_t count, int is_descending);
status_t write_employees(const char *filename, const Employee *arr, size_t count);
void free_employees(Employee *arr, size_t count);

#endif