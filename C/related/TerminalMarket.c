#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>


typedef struct register_element
{
    int *value;
    int selfindex;
}
register_element;

register_element product_quantity = {NULL, 0};
register_element product_index = {NULL, 0};

typedef struct products
{
    char *name;
    double price;
}
products;

products catalog[5] = {{"Milk\0"     , 1.35},
                       {"Butter\0"   , 4.50}, 
                       {"Coffee\0"   , 2.55}, 
                       {"Chocolate\0", 3.75}, 
                       {"Suggar\0"   , 5.00}};

const int biggeststring = 11;   //buffer needs to have the same size as the biggest string between the products + \n from fgets
int catalogsize = sizeof(catalog) / sizeof(products);

int printbill();

int securitycheck(char*);
int productcheck(char*);
int register_manager(int, int**, int*);

double billcalculation();

char* inputf();

int main()
{

    printbill();

    int freturnvalue = 0;

    double money = 200;

    bool validinput = false;
    bool repeat = false;

    printf("Welcome to the Terminal Market!\n");
    printf("Now, you have 200 bucks\n");
    
    printf("Here is our catalog -\n");
    for(int i = 0; i < catalogsize; i++)
    {
        printf("%s:\t\t%.2lf\n", catalog[i].name, catalog[i].price);
    }

    do
    {
        printf("What product do you want?\n");
        printf("(If you don't want to buy something type 'Exit')\n");

        while(validinput == false)
        {
            char *userinput = inputf();

            if(securitycheck(userinput) == 1)
            {
                free(userinput);
                return 0;
            }
            else if(securitycheck(userinput) == -1)
            {
                free(userinput);
                continue;
            }
            

            if(productcheck(userinput) == 0)
            {
                free(userinput);
                break;
            }
            else
            {
                printf("Enter a valid product or command\n");
                free(userinput);
            }
        }

        printf("How much of it do you want?(maximum of 99 products)\n");
        do
        {
            int temp;
            if(freturnvalue == 1)
            {
                printf("An error ocourred. Enter the quantity of the product again\n");
                printf("Also, make sure to enter only numbers\n");
            }

            char *userinput = inputf();
            if(isdigit(userinput[0]) == false && isdigit(userinput[1]) == false)
            {
                freturnvalue = 1;
            }
            else
            {
                sscanf(userinput, "%d", &temp);  
                free(userinput);

                freturnvalue = register_manager(temp, &(product_quantity.value), &(product_quantity.selfindex));
            }
        }
        while(freturnvalue == 1);
        
        printf("Do you want to buy another product?(Type y/n)\n");
        char *userinput = inputf();
        if(strcmp(userinput, "Y") == 0)
        {
            repeat = true;
            free(userinput);
        } 
        else
        {
            free(userinput);
            break;
        }
    }
    while(repeat == true);

    printf("The bill will be generated in a text file in your desktop.\n");

    while(printbill() == 1)
    {
        printf("Failed to generate bill, we will try again\n");
    }

    printf("Done.\n");
    
    return 0;
}

int printbill()
{
    FILE *pBillfile = fopen("C:\\Users\\ender\\OneDrive\\Área de Trabalho\\Bill.txt", "w");

    if(pBillfile == NULL)
    {
        return 1;
    }

    fprintf(pBillfile, "***********************\n");
    fprintf(pBillfile, "    TERMINAL MARKET\n");
    fprintf(pBillfile, "***********************\n\n");

    fprintf(pBillfile, "- You bought -\n");

    for(int i = 0; i < product_index.selfindex; i++)
    {
        double purchase_totalprice =  product_quantity.value[i] * catalog[product_index.value[i]].price;

        fprintf(pBillfile, "(%dx) %s:\t\t%.2lf\n", product_quantity.value[i], catalog[product_index.value[i]].name, purchase_totalprice);

        purchase_totalprice = 0;
    }

    fprintf(pBillfile, "\nTotal price: %.2lf\n", billcalculation());
    fprintf(pBillfile, "Thanks for buying in the Terminal Market!\n");

    fclose(pBillfile);

    return 0;
}

int securitycheck(char *userinput)
{
    if(strcmp(userinput, "Exit") == 0)
    {
        return 1;
    }
    else if(userinput == NULL)
    {
        printf("An error has ocourred. Enter your product again\n");
        return -1;
    }

    return 0;
}

int productcheck(char *userinput)
{
    for(int i = 0; i < catalogsize; i++)
    {
        if(strcmp(userinput, catalog[i].name) == 0)
        {
            register_manager(i, &(product_index.value), &(product_index.selfindex));
            return 0;
        }
    }
    return 1;
}

int register_manager(int input, int **register_element, int *index)
{
    if((*index) > 0)
    {
        int *temp = realloc(*register_element, ((*index) + 1) * sizeof(int));

        if(temp == NULL)
        {
            free(temp);
            return 1;
        }
        else
        {
            *register_element = temp;
            temp = NULL;
        }

        (*register_element)[(*index)] = input;
        (*index)++;
    }
    else
    {
        *register_element = calloc((*index) + 1, sizeof(int));

        if(*register_element == NULL)
        {
            return 1;
        }

        (*register_element)[(*index)] = input;

        (*index)++;
    }

    return 0;
}

double billcalculation()
{
    double bill = 0;

    for(int i = 0; i < product_index.selfindex; i++)
    { 
        bill += (product_quantity.value[i]) * (catalog[(product_index.value[i])].price);
    }

    return bill;
}

char* inputf()
{
    char *buffer = malloc(biggeststring);

    if(buffer == NULL)
    {
        return NULL;
    }   

    if(fgets(buffer, biggeststring, stdin) == NULL)
    {
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    buffer[0] = toupper(buffer[0]);

    return buffer;
}