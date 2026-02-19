#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void numberofdigits(int *digits, int number)
{
    for(*digits; (*digits) < 15; (*digits)++)
    {   
        number = number / 10;

        if(number < 1)
        {
            (*digits)++;
            break;
        }
    }
}

int main()
{
    FILE *pFilewrite;
    FILE *pTempfile;

    char filename[75] = "C:\\Users\\ender\\OneDrive\\Área de Trabalho\\";
    char name[15];
    char content[255];
    
    int filenamesize;
    int digits = 0;


    printf("Write the name and the format for a file(name.format): ");

    fgets(name, 15, stdin);
    name[strcspn(name, "\n")] = '\0';

    strcat(filename, name);
    filenamesize = strcspn(filename, "\0");

    numberofdigits(&digits, filenamesize);

    pTempfile = fopen("temp.txt", "w");

    if(pTempfile == NULL)
    {
        printf("Failed to open file.");
        return 1;
    }
    fprintf(pTempfile, "%d\n%s", filenamesize, filename);

    fclose(pTempfile);

    printf("Write a content for the file:\n");

    fgets(content, 255, stdin);

    pFilewrite = fopen(filename, "w");

    if(pFilewrite == NULL)
    {
        printf("Failed to open file.");
        return 1;
    }
    fprintf(pFilewrite, content);

    fclose(pFilewrite);

    return 0;
}