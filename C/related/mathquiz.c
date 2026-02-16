#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void coeficientsgenerator(double[3]);
void getanswer(double[3]);
void compareanswerandresponse(double[3], double);

int main()
{
    srand(time(0));

    bool _continue = true;
    char continueresponse;
    double trueanswer;
    double useranswer[3];
    double coeficients[3];

    printf("Lets do some math!\n");
    while(_continue)
    {
        coeficientsgenerator(coeficients);
        trueanswer = (coeficients[2] - coeficients[1])/coeficients[0];

        printf("What's the answer of this equation(first, enter the numerator, and then the denominator):\n");
        printf("%.1lfx + %.1lf = %.1lf?\n", coeficients[0], coeficients[1],  coeficients[2]);
        do
        {
            getanswer(useranswer);
            compareanswerandresponse(useranswer, trueanswer);
        } 
        while (useranswer[2] != trueanswer);
   
        printf("Wanna try another equation?(y/n) ");
        scanf("%2s", &continueresponse);
        (continueresponse == 'y') ? (_continue) : (_continue = false);
    }

    printf("Bye!");

    return 0;
}

void coeficientsgenerator(double coeficients[3]){
    coeficients[0] = (rand() % 50) + 1;
    coeficients[1] = (rand() % 50) + 1;
    coeficients[2] = (rand() % 50) + 1;
}

void compareanswerandresponse(double useranswer[3], double trueanswer)
{
    (useranswer[2] != trueanswer) ? (printf("You're wrong! Try again: ")) : (printf("You're right!\n"));
}

void getanswer(double useranswer[3])
{
    scanf(" %lf", &useranswer[0]);
    printf("/");
    scanf("%lf", &useranswer[1]);
    useranswer[2] = useranswer[0] / useranswer[1];
}