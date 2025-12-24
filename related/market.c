#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(){

    char catalog[5][10] = {"Milk", "Butter", "Chocolate", "Suggar", "Coffee"};
    char reg[][10] = {};
    char answer1[5];
    char answer2[10];
    double money = 200;
    double prices[5] = {9.00, 3.50, 1.75, 3.10, 5.00};

    if(sizeof(catalog)/sizeof(catalog[0]) == sizeof(prices)/sizeof(prices[0]))
    {
        printf("Welcome to the C virtual market!\n");
        printf("Currently, you have 200 bucks.\n");
        printf("Do you want to enter the market or exit?(type 'enter' or 'exit') ");
        scanf("%s", &answer1);

        if(strcmp(answer1, "enter") == 0)
        {
            printf("Here is our catalog - \n");
            for(int i = 0; i < sizeof(catalog)/sizeof(catalog[0]); i++)
            {
                printf("%s: $%.2lf\n", catalog[i], prices[i]);
            }

            printf("What products do you want to buy?\n");
            for(int j = 0; j < sizeof(catalog)/sizeof(catalog[0]); j++)
            {
                fgets(answer2, sizeof(catalog), stdin);
                answer2[strlen(answer2) - 1] = '\0';

                strcpy(reg[j], answer2);

                for(int k = 0; k < sizeof(catalog)/sizeof(catalog[0]); k++)
                {
                    if(strcmp(answer2, catalog[k]) == 0)
                    {
                        money -= prices[k];
                    }
                }
            }

            printf("You spent $%.2lf on products.\n", 200 - money);
            printf("Here is what you bought:");
            for(int l = 0; l < sizeof(catalog)/sizeof(catalog[0]); l++)
            {
                printf("%s\n", reg[l]);
            }

            printf("Now, you have %.2lf", money);
            

        }
        else
        {
            printf("Bye.");
        }
    }
    else
    {
        printf("Error: Number of items aren't matching with the number of prices or/and indexes");
    }


    return 0;
}