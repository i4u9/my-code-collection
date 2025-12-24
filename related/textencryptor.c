#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void printarray(int textsize, int array[textsize])
{
    for(int i = 0; i < textsize; i++)
    {
        printf("%d", array[i]);
    }
    printf("\n");
}

int main()
{
    char text[101];
    int textsize = 0;

    printf("Type a text to encrypt(max. of 100 characters):\n");
    fgets(text, 101, stdin);

    for(textsize; textsize < 101; textsize++)
    {
        if(text[textsize] == '\0') {break;} 
    }

    char decryptedtext[textsize];
    int enccharacters[textsize];

    for(int i = 0; i < textsize; i++)
    {
        enccharacters[i] = (int)text[i];
    }
    strcpy(text, "");
    for(int i = 0; i < textsize; i++)
    {
        decryptedtext[i] = (char)enccharacters[i];
    }
    decryptedtext[textsize + 1] = '\0';

    printf("Here is your text after being encrypted:\n");
    printarray(textsize, enccharacters);

    printf("Here is the original text:\n");
    printf("%s\n", decryptedtext);

    return 0;
}