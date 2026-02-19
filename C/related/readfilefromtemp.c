#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *pFilename;
    FILE *pFileread;

    int filenamesize;

    char buffer[255];
    char *temp;

    pFilename = fopen("temp.txt", "r");

    if(pFilename == NULL)
    {
        printf("Failed to open file.");
        return 1;
    }

    fgets(temp, 4, pFilename);
    filenamesize = atoi(temp);

    char filename[filenamesize];

    fgets(filename, filenamesize + 1, pFilename);

    fclose(pFilename);

    pFileread = fopen(filename, "r");

    if(pFileread == NULL)
    {
        printf("Failed to open file.");
        return 1;
    }
    fgets(buffer, 255, pFileread);

    fclose(pFileread);

    printf("%s", buffer);

    return 0;
}