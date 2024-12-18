#pragma once

template <class T>
int count_digits(T number)
{
    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}