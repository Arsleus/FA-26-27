#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum {
    StatusSuccess,
    StatusInvalidParameter,
    StatusOverflow
} FuncStatus;

typedef enum {
    TypePrime,
    TypeComposite,
    TypeNeither
} NumberType;

FuncStatus ParseLong(const char *str, long *result) {
    if (str == NULL || *str == '\0') {
        return StatusInvalidParameter;
    }
    char *endptr;
    long value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return StatusInvalidParameter;
    }
    if (value == LONG_MAX || value == LONG_MIN) {
        return StatusOverflow;
    }
    *result = value;
    return StatusSuccess;
}

FuncStatus FindMultiplies(long x, int *buffer, int *count) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    int local_count = 0;
    for (int i = 1; i <= 100; ++i) {
        if (i % x == 0) {
            buffer[local_count] = i;
            local_count++;
        }
    }
    *count = local_count;
    return StatusSuccess;
}

FuncStatus CheckPrime(long x, NumberType *result) {
    if (x<= 0) {
        return StatusInvalidParameter;
    }
    if (x == 1) {
        *result = TypeNeither;
        return StatusSuccess;
    }
    if (x == 2) {
        *result = TypePrime;
        return StatusSuccess;
    }
    if (x % 2 == 0) {
        *result = TypeComposite;
        return StatusSuccess;
    }
    for (long i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            *result = TypeComposite;
            return StatusSuccess;
        }
    }
    *result = TypePrime;
    return StatusSuccess;
}

FuncStatus SplitToHexDigits(long x, int *digits_buffer, int *count) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    int local_count = 0;
    while(x > 0) {
        digits_buffer[local_count] = x % 16;
        x /= 16;
        local_count++;
    }
    *count = local_count;
    return StatusSuccess;
}

FuncStatus CreatePowersTable(long x, long long table[10][10]) {
    if ( x<= 0 || x > 10) {
        return StatusInvalidParameter;
    }
    for (int base = 1; base <= 10; ++base) {
        long long current_power = 1;
        for (int exp = 1; exp <= x; ++exp) {
            current_power *= base;
            table[base - 1][exp - 1] = current_power;
        }
    }
    return StatusSuccess;
}

FuncStatus CalculateNaturalSum(long x, long long *sum) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    if (x > 3000000000LL) {
        return StatusOverflow;
    }
    *sum = ((long long)x * (x + 1)) / 2;
    return StatusSuccess;
}

FuncStatus CalculateFactorial(long x, long long *factorial) {
    if (x <= 0) {
        return StatusInvalidParameter;
    }
    if (x > 20) {
        return StatusOverflow;
    }
    long long current_factorial = 1;
    for (int i = 1; i <= x; ++i) {
        current_factorial *= i;
    }
    *factorial = current_factorial;
    return StatusSuccess;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Ошибка: Неверное количество аргументов.\n");
        return 1;
    }
    long x = 0;
    FuncStatus parse_status = ParseLong(argv[1], &x);
    if (parse_status == StatusInvalidParameter) {
        printf("Ошибка: Ошибочный формат числа '%s'.\n", argv[1]);
        return 1;
    } else if (parse_status == StatusOverflow) {
        printf("Ошибка: Число '%s' вызвало переполнение памяти.\n", argv[1]);
        return 1;
    }
    char *flag_str = argv[2];
    if ((flag_str[0] != '-' && flag_str[0] != '/') || flag_str[1] == '\0' || flag_str[2] != '\0') {
        printf("Ошибка: Неверный формат флага '%s'.\n", flag_str);
        return 1;
    }
    char action = flag_str[1];
    switch (action) {
        case 'h': {
            int buffer[100];
            int count = 0;
            if (FindMultiplies(x, buffer, &count) == StatusInvalidParameter) {
                printf("Ошибка: Для флага -h число должно быть строго больше 0.\n");
                return 1;
            }
            if (count == 0) {
                printf("Натуральных чисел, кратных %ld в пределах 100, не найдено.\n", x);
            } else {
                for (int i = 0; i < count; ++i) {
                    printf("%d ", buffer[i]);
                }
                printf("\n");
            }
            break;
        }
        case 'p': {
            NumberType result;
            if (CheckPrime(x, &result) == StatusInvalidParameter) {
                printf("Ошибка: Для флага -p число должно быть строго больше 0.\n");
                return 1;
            }
            if (result == TypePrime) {
                printf("Число %ld является простым.\n", x);
            } else if (result == TypeComposite) {
                printf("Число %ld является составным.\n", x);
            } else {
                printf("Число %ld не является ни простым, ни составным.\n", x);
            }
            break;
        }
        case 's': {
            int digits_buffer[20];
            int count = 0;
            if (SplitToHexDigits(x, digits_buffer, &count) == StatusInvalidParameter) {
                printf("Ошибка: Для флага -s число должно быть строго больше 0.\n");
                return 1;
            }
            const char hex_chars[] = "0123456789ABCDEF";
            for (int i = count - 1; i >= 0; --i) {
                printf("%c ", hex_chars[digits_buffer[i]]);
            }
            printf("\n");
            break;
        }
        case 'e': {
            long long table[10][10];
            if (CreatePowersTable(x, table) == StatusInvalidParameter) {
                printf("Ошибка: Для флага -e число должно быть в диапазоне от 1 до 10.\n");
                return 1;
            }
            for (int base = 0; base < 10; ++base) {
                for (int exp = 0; exp < x; ++exp) {
                    printf("%lld ", table[base][exp]);
                }
                printf("\n");
            }
            break;
        }
        case 'a': {
            long long sum = 0;
            FuncStatus status = CalculateNaturalSum(x, &sum);
            if (status == StatusInvalidParameter) {
                printf("Ошибка: Для флага -a число должно быть строго больше 0.\n");
                return 1;
            } else if (status == StatusOverflow) {
                printf("Ошибка: Сумма для такого большого числа не помещается в память.\n");
                return 1;
            }
            printf("Сумма всех натуральных чисел от 1 до %ld равна: %lld\n", x, sum);
            break;
        }
        case 'f': {
            long long factorial = 0;
            FuncStatus status = CalculateFactorial(x, &factorial);
            if (status == StatusInvalidParameter) {
                printf("Ошибка: Для флага -f число должно быть строго больше 0.\n");
                return 1;
            } else if (status == StatusOverflow) {
                printf("Ошибка: Факториал для числа %ld превышает лимит типа данных (x > 20).\n", x);
                return 1;
            }
            printf("Факториал числа %ld равен: %lld\n", x, factorial);
            break;
        }
        default:
            printf("Ошибка: Неизвестное действие '-%c'.\n", action);
            return 1;
    }
    return 0;
}

