#include "lab1fa.h"

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
            int has_multiplies = 0;
            FindMultiplies(x, &has_multiplies);  
            if (!has_multiplies) {
                printf("Натуральных чисел, кратных %ld в пределах 100, не найдено.\n", x);
            } else {
                for (int i = (int)x; i <= 100; i += (int)x) {
                    printf("%d ", i);
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
            int digits_buffer[16];
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
            // Выделяем на стеке ровно столько памяти, сколько запросил пользователь (10 * x)
            long long table[10 * x];
            if (CreatePowersTable((int)x, table) == StatusInvalidParameter) {
                printf("Ошибка: Для флага -e число должно быть в диапазоне от 1 до 10.\n");
                return 1;
            }
            for (int base = 0; base < 10; ++base) {
                for (int exp = 0; exp < x; ++exp) {
                    printf("%lld ", table[base * (int)x + exp]);
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
                printf("Ошибка: Переполнение.\n");
                return 1;
            }
            printf("Сумма всех натуральных чисел от 1 до %ld равна: %lld\n", x, sum);
            break;
        }
        case 'f': {
            long long factorial = 0;
            FuncStatus status = CalculateFactorial((int)x, &factorial);
            if (status == StatusInvalidParameter) {
                printf("Ошибка: Для флага -f число должно быть строго больше 0.\n");
                return 1;
            } else if (status == StatusOverflow) {
                printf("Ошибка: Переполнение.\n");
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



