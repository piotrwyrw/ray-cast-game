#include "util.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define IS_DIGIT(c) (c >= '0' && c <= '9')

static int c_conv_int(const char c)
{
        if (!IS_DIGIT(c))
                return -1;

        return c - '0';
}

_Bool str_conv_d(const char *str, double *dst)
{
        *dst = 0.0;

        unsigned long len = strlen(str);

        double d = 0.0;

        _Bool negative = false;

        _Bool decimal = false;
        double decimal_i = 0; // Decimal place index

        unsigned long i = 0;
        char c = str[i];

        if (c == '-') {
                negative = true;
                i++;
        }

        for (; i < len; i++) {
                c = str[i];

                if (!IS_DIGIT(c)) {
                        if (c == '.') {
                                if (!decimal) {
                                        decimal = true;
                                        continue;
                                } else {
                                        // Multiple decimal points are not allowed
                                        return false;
                                }
                        }

                        // Illegal character
                        return false;
                }

                int n = c_conv_int(c);

                // This shouldn't happen because of the above check, but let's handle it anyway
                if (n < 0)
                        return false;

                if (!decimal)
                        d = d * 10 + n;
                else
                        d += (n * pow(10, -(++decimal_i)));
        }

        *dst = negative ? (d * -1.0) : d;
        return true;
}