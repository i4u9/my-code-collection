#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char sset[10];
    int elements[5];
} set;

void printelements(set set);

void bubblesort(int m, int sunion[]);

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{

    set set1;
    set set2;
    set intersection;
    int k = 0;
    int sunion[10];
    int m = 0;
    int sunioncpy[10];
    int n = 0;

    printf("Enter 5 elements to 2 sets -\n");
    printf("Set 1: ");
    for(int i = 0; i < 5; i++)
    {
        fgets(set1.sset, 10, stdin);
        if(sizeof(set1.elements[i]) == 0)
        {
            i--;
            continue;
        }
    }
    printf("Set 2: ");
    for(int i = 0; i < 5; i++)
    {
        fgets(set2.sset, 10, stdin);
        sscanf(set2.sset, "%d", &set2.elements[i]);
    }

    printf("Set 1: ");
    printelements(set1);
    printf("Set 2: ");
    printelements(set2);

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(set1.elements[i] == set2.elements[j])
            {
                intersection.elements[k] = set1.elements[i];
                k++;
                break;
            }
        }
    }

    for(int i = 0; i < 5; i++)
    {
        sunion[m] = set1.elements[i];
        m++;
        sunion[m] = set2.elements[i];
        m++;
    }
    bubblesort(m, sunion);
    for(int i = 0; i < m; i++)
    {
        if(sunion[i] == sunion[i + 1])
        {
            sunioncpy[n] = sunion[i];
            n++;
            i++;
        }
        else
        {
            sunioncpy[n] = sunion[i];
            n++;
        }
    }
    

    printf("Intersection of set 1 with set 2: ");
    for(int i = 0; i < k; i++)
    {
        printf("%d ", intersection.elements[i]);
    }
    printf("\n");
    printf("Union of set 1 with set 2: ");
    for(int i = 0; i < n; i++)
    {
        printf("%d ", sunioncpy[i]);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void printelements(set set)
{
    for(int i = 0; i < 5; i++)
    {
        printf("%d ", set.elements[i]);
    }
    printf("\n");
}

void bubblesort(int m, int sunion[])
{
   int i = 0;
   bool swapped;
   do
   {
        swapped = false;
        for(int j = 0; j < m - 1 - i; j++)
        {
            if(sunion[j] > sunion[j + 1])
            {
                int temp = sunion[j];
                sunion[j] = sunion[j + 1];
                sunion[j + 1] = temp;
                swapped = true;
            }
        }
   } while (swapped);

}