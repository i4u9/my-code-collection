#include "testcontrol.h"
#include <sys/time.h>

int main()
{
    struct timeval start, end;

    int digits = 7;     //9999999 (1000000)
    char command[7 + digits];
    strcpy(command, "test ");

    FILE *pResultfile = fopen("result.txt", "w");
    if (pResultfile == NULL)    {   return 1;   }
    fclose(pResultfile);

    gettimeofday(&start, NULL);
    for (int i = 1; i <= lines_limit; i++)
    {
        char string[digits];
        strcat(command, itoa(i, string, 10));

        system(command);
        strcpy(command, "test "); 
    }
    gettimeofday(&end, NULL);

    printf("\nTests were runned successfully\n");
    printf ("Total time execution: %lf seconds\n", (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec));
    
    return 0;
}