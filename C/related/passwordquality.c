#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

enum enumwhatnumberof{integers, symbols, characters};

void sizefbuffer(int stringsize, char strinput[stringsize], int *inputsize)
{
    for(*inputsize; (*inputsize) < stringsize; (*inputsize)++)     
    {
        if(strinput[(*inputsize)] == '\0')    {break;}
    }
}

void strfind(int size, char strinput[size], int (*pNumebrof)[3])
{
    for(int i = 0; i < (size - 1); i++)
    {
        if(48 <= strinput[i] && strinput[i] <= 57)
        {   ((*pNumebrof)[integers])++;  }

            else if((32 <= strinput[i] && strinput[i] <= 47) || (58 <= strinput[i] && strinput[i] <= 64) || (91 <= strinput[i] && strinput[i] <= 96) || (123 <= strinput[i] && strinput[i] <= 126))
            {   ((*pNumebrof)[symbols])++;   }

            else
            {   ((*pNumebrof)[characters])++;}
    }
}

void seclevel(int numberof[3], int (*securitylevelof)[2])
{
    if(numberof[integers] <= 1)
    {
        (*securitylevelof)[integers] = 1;
    }
    else if(numberof[integers] > 1 && numberof[integers] <= 4)
    {
        (*securitylevelof)[integers] = 2;
    }
    else
    {
        (*securitylevelof)[integers] = 4;
    }

    if(numberof[symbols] <= 2)
    {
        (*securitylevelof)[symbols] = 1;
    }
    else if(numberof[symbols] > 2 && numberof[symbols] <= 4)
    {
        (*securitylevelof)[symbols] = 2;
    }
    else
    {
        (*securitylevelof)[symbols] = 4;
    }
}

int main()
{   
    bool minimumcondition = false;

    char strinput[26];

    int size = sizeof(strinput);
    int inputsize = 0;
    
    int minimumof[2] = {2, 1};
    int numberof[3] = {0, 0, 0};
    int securitylevelof[2] = {0, 0};


    printf("Enter a password with a maximum of 25 characters(remember to include at least %d numbers and %d symbols - !, @, , #, etc -)\n", minimumof[integers], minimumof[symbols]);
    fgets(strinput, 26, stdin);
    strinput[strcspn(strinput, "\n")] = '\0';

    do  
    {
        strfind(size, strinput, &numberof);
        sizefbuffer(size, strinput, &inputsize);

        if(numberof[integers] < minimumof[integers] || numberof[symbols] < minimumof[symbols])
        {
            printf("Enter a valid password with at least %d numbers and %d symbols - !, @, , #, etc -\n", minimumof[integers], minimumof[symbols]);
            fgets(strinput, 26, stdin);
            strinput[strcspn(strinput, "\n")] = '\0';
        }
        else    
        { 
            minimumcondition = true;
        }
    }
    while(minimumcondition == false);

    seclevel(numberof, &securitylevelof);
    float sec = (securitylevelof[integers] + securitylevelof[symbols]) / (float)2;

    printf("The security level of you password is: ");
    if(sec <= 1)
    {
        printf("weak");
    }
    else if(sec > 1 && sec <= 3)
    {
        printf("medium");
    }
    else
    {
        printf("strong");
    }    

    return 0;
}
