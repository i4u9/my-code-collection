#include <stdio.h>
// #include <windows.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

const int Maxinput = 3; //2 digits number + /0

bool display = true;

enum clkelements{hours, minutes, seconds};

void displaymanager(int *, int *);
int getvalue(char[8]);

int main()
{   
    char buffer[3] = {0};

    printf("Do you want to access the clock or set a timer?(type c/t): ");
    fgets(buffer, 3, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    if(strcmp(buffer, "t") == 0)
    {
        int mainbreaker = 0;
        int breakers[3] = {0, 0, 0}; 
        int values[3] = {0, 0, 0};

        printf("Insert the values for the timer -\n");
        breakers[hours] = getvalue("Hours  ");
        breakers[minutes] = getvalue("Minutes");
        breakers[seconds] = getvalue("Seconds");

        while(display)
        {
            displaymanager(&values[seconds], &values[minutes]);
            displaymanager(&values[minutes], &values[hours]);
            displaymanager(&values[hours], &mainbreaker);

            if(mainbreaker == 1)    
            {
                break;
            }

            printf("%02d:%02d:%02d\n", values[hours], values[minutes], values[seconds]);

            if(values[hours] == 0)
            {}
            else if(values[hours] == breakers[hours])    
            {
                break;
            }    

            if(values[minutes] == 0)
            {}
            else if(values[minutes] == breakers[minutes])    
            {
                break;
            }

            if(values[seconds] == 0)
            {}
            else if(values[seconds] == breakers[seconds])    
            {
                break;
            }

            values[seconds]++;
            sleep(1);
        }
    }
    else
    {
        time_t rawtime = 0;
        struct tm *pTime = NULL;

        while(display)
        {
            time(&rawtime);

            pTime = localtime(&rawtime);

            printf("\r%02d:%02d:%02d\n", pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
            sleep(1);
        }
    }

    printf("Bye");

    return 0;
}

void displaymanager(int *clockelement, int *nextclockelement)
{
    if(*clockelement > 59)
    {
        *clockelement = 0;
        (*nextclockelement)++;
    }
}

int getvalue(char stringelement[8])
{
    int elementbreaker = 0;
    char temp[Maxinput];

    printf("%s: ", stringelement);
    fgets(temp, Maxinput, stdin);
    temp[strcspn(temp, "\n")] = '\0';

    sscanf(temp, "%d", &elementbreaker);

    return elementbreaker;
}