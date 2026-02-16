#include <stdio.h>
#include <stdlib.h>

int get_int_digits(int integer)
{
    int digits = 0;
    while ((int)(integer / 10) > 0)
    {
        integer /= 10;
        digits++;
    }
    return digits + 1;
}