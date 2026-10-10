#ifndef LAB1_8_H
#define LAB1_8_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum {
    StatusSuccess,
    StatusInvalidCharacter, 
    StatusOverflow 
} ParseStatus;

int GetSymbolValue(char c);
ParseStatus ProcessNumberString(const char *str, int *min_base, long long *val_dec);

#endif
