#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void userstringinput(char userstring[100])
{
    fgets(userstring, 100, stdin);
    userstring[strcspn(userstring, "\n")] = '\0';
}

int main()
{
    char userstring[100];
    int size = 0;
    char password[11];
    char response[11];

    printf("Type a text:\n");
    userstringinput(userstring);

    for(size; size < 100; size++)
    {
        if(userstring[size] == '\0') {size++; break;} 
    }

    int buffer = (size / 2) + 1;

    char rightpart[buffer];
    char leftpart[buffer];

    strncpy(leftpart, userstring, buffer - 1);
    leftpart[buffer - 1] = '\0';
    strncpy(rightpart, userstring + (buffer - 1), buffer - 1); 
    
    printf("Enter a password(max. of 10 characters or numbers) to secure this text:\n");
    fgets(password, 11, stdin);

    printf("This is a fragment of your text:\n");
    printf("%s\n", leftpart);

    printf("Type the password to see the rest(if you miss, the text will be deleted): ");
    fgets(response, 11, stdin);

    if(strcmp(password, response) == 0)
    {
        printf("Here is the text:\n");
       printf("%s\n", leftpart);
       printf("%s\n", rightpart);
    }
    else
    {
        printf("You lost your text");
    }

    return 0;
}
