#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INPUTFILE_MAXSIZE 250
#define MAX_BUFFER 10
#define MAX_NUMBER 99999
#define MAX_DIGITS 5

int inputf(char*, int);
int inputf_number(bool);
void inputf_number_reset(char*, bool*);
bool inputf_number_check(char*, int, int, bool*);

int main()
{
    int number = 0;

    number = inputf_number(true);

    printf("%d", number);
    
    return 0;
}

int inputf(char *dest_buffer, int count)
{
    FILE *pInputfile_write = fopen("input.txt", "w+");
    if(pInputfile_write == NULL)
    {
        return 1;
    }

    char tempstr[INPUTFILE_MAXSIZE];
    fgets(tempstr, INPUTFILE_MAXSIZE, stdin);
    //tempstr[strcspn(tempstr, "\n")] = '\0';
    fprintf(pInputfile_write, "%s", tempstr);

    FILE *pInputfile_read = freopen("input.txt", "r", pInputfile_write);
    if (pInputfile_read == NULL)
    {
        return 1;
    }

    fgets(dest_buffer, count + 2, pInputfile_read); //  include '-' char and keep MAX_DIGITS
    dest_buffer[strcspn(dest_buffer, "\n")] = '\0';

    FILE *tempptr = freopen("input.txt", "w", pInputfile_read);
    if (tempptr == NULL)
    {
        return 1;
    }

    fclose(tempptr);
    return 0;
}

int inputf_number(bool allow_negnumber)
{
    int temp = 0;
    char buffer[MAX_BUFFER];

    bool valid_number = false;

    while(valid_number == false)
    {
        int local_temp = 0;

        while(inputf(buffer, MAX_DIGITS) == 1)
        {
            printf("An error occoured, enter the number again\n");
        }
        int strlenght = strcspn(buffer, "\n");
        buffer[strlenght] = '\0';

        bool is_negnumber = (buffer[0] == '-' && buffer[1] != '\0');

        if(allow_negnumber == true && is_negnumber == true)
        {
            inputf_number_check(buffer, 1, strlenght, &valid_number);
        }
        else
        {
            inputf_number_check(buffer, 0, strlenght, &valid_number);
        }

        if(valid_number == false)
        {
            printf("Invalid number\n");
            inputf_number_reset(buffer, &valid_number);
        }
        else
        {
            local_temp = atoi(buffer);

            if(abs(local_temp) > MAX_NUMBER)
            {
                printf("Number out of range.\n");
                inputf_number_reset(buffer, &valid_number);
            }
            else
            {
                temp = local_temp;
                break;
            }
        }
    }

    return temp;
}

void inputf_number_reset(char *buffer, bool *element2)
{
    buffer[0] = '\0';
    *element2 = false;
}

bool inputf_number_check(char *string, int index, int strlenght, bool *condition)
{
    for(int i = index; i < strlenght; i++)
    {
        if(isdigit(string[i]) == false)
        {
            *condition = false;
            break;
        }
        else
        {
            *condition = true;
        }
    }
}