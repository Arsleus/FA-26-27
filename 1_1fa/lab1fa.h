#ifndef LAB1FA_H
#define LAB1FA_H

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

FuncStatus ParseLong(const char *str, long *result);
FuncStatus FindMultiplies(long x, int *has_multiplies);
FuncStatus CheckPrime(long x, NumberType *result);
FuncStatus SplitToHexDigits(long x, int *digits_buffer, int *count);
FuncStatus CreatePowersTable(int x, long long *table);
FuncStatus CalculateNaturalSum(long x, long long *sum);
FuncStatus CalculateFactorial(int x, long long *factorial);

#endif
