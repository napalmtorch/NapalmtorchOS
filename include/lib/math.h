#pragma once
#include <lib/types.h>

static inline double powd(double x, int y)
{
    double temp;
    if (y == 0)
    return 1;
    temp = powd (x, y / 2);
    if ((y % 2) == 0) {
        return temp * temp;
    } else {
        if (y > 0)
            return x * temp * temp;
        else
            return (temp * temp) / x;
    }
}