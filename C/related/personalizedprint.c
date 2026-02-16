#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

void print(char string[], int extimeinsec)
{
    printf("%s\n", string);
    Sleep(1000 * extimeinsec);
}

int main()
{   system("cls");

    print("sgsfgbaho", 3);
    print("mçp[1]", 5);

    return 0;
}