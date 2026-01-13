#pragma once

constexpr size_t ipow(double base, int exp)
{
    if (exp == 0)
    {
        return 1;
    }
    if (exp < 0)
    {
        return 0;
    }

    if (exp % 2 == 0)
    {
        size_t half = ipow(base, exp / 2);
        return half * half;
    }

    return base * ipow(base, exp - 1);
}
