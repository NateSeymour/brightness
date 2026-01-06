#ifndef DOORCTL_MATH_H
#define DOORCTL_MATH_H

#include "pico/stdlib.h"

inline size_t min(size_t a, size_t b)
{
    if (a < b) return a;
    return b;
}

#endif //DOORCTL_MATH_H