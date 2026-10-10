#include "9lab1pac.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

status_t process_fixed_array(int *arr, size_t size, int *out_min, int *out_max) {
    if (arr == NULL || out_min == NULL || out_max == NULL || size == 0) {
        return STATUS_ERR_NULL_ARG;
    }

    size_t min_idx = 0;
    size_t max_idx = 0;

    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }

    *out_min = arr[min_idx];
    *out_max = arr[max_idx];

    swap(&arr[min_idx], &arr[max_idx]);

    return STATUS_OK;
}

static int cmp_int(const void *a, const void *b) {
    int val_a = *(const int *)a;
    int val_b = *(const int *)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

static int find_nearest(int *arr, size_t size, int target) {
    size_t left = 0;
    size_t right = size - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return arr[mid];
        }
        
        if (arr[mid] < target) {
            if (mid == size - 1) break;
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }

    int val_left = (left < size) ? arr[left] : INT_MIN;
    int val_right = (right < size) ? arr[right] : INT_MAX;
    
    int diff_left = (val_left != INT_MIN) ? abs(val_left - target) : INT_MAX;
    int diff_right = (val_right != INT_MAX) ? abs(val_right - target) : INT_MAX;

    return (diff_left <= diff_right) ? val_left : val_right;
}

status_t process_dynamic_arrays(size_t *out_size, int **out_array_c) {
    if (out_size == NULL || out_array_c == NULL) {
        return STATUS_ERR_NULL_ARG;
    }
    *out_size = 0;
    *out_array_c = NULL;

    size_t n_a = (size_t)(rand() % 9991 + 10);
    size_t n_b = (size_t)(rand() % 9991 + 10);

    int *a = (int *)malloc(n_a * sizeof(int));
    if (a == NULL) return STATUS_ERR_MEMORY;

    int *b = (int *)malloc(n_b * sizeof(int));
    if (b == NULL) {
        free(a);
        return STATUS_ERR_MEMORY;
    }

    int *c = (int *)malloc(n_a * sizeof(int));
    if (c == NULL) {
        free(a);
        free(b);
        return STATUS_ERR_MEMORY;
    }

    for (size_t i = 0; i < n_a; i++) {
        a[i] = rand() % 2001 - 1000;
    }

    for (size_t i = 0; i < n_b; i++) {
        b[i] = rand() % 2001 - 1000;
    }

    qsort(b, n_b, sizeof(int), cmp_int);

    for (size_t i = 0; i < n_a; i++) {
        int nearest = find_nearest(b, n_b, a[i]);
        c[i] = a[i] + nearest;
    }

    free(a);
    free(b);

    *out_size = n_a;
    *out_array_c = c;
    return STATUS_OK;
}

void free_dynamic_array(int *arr) {
    if (arr != NULL) {
        free(arr);
    }
}