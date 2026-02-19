#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
    char input[20];

    int number = 0;
    int numberofdigits = 0;

    printf("Enter an integer:");
    fgets(input, 20, stdin);

    sscanf(input, "%d", &number);

    double temp = (double)number;
    for(numberofdigits; numberofdigits < INT_MAX; numberofdigits++)
    {
        if(temp < 1)
        {
            break;
        }

        temp /= (double)10;
    }

    printf("The number %d have %d digits", number, numberofdigits);

    return 0;
}