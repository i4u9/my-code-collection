#include "testcontrol.h"

void generate_randnumbers(int*, int, int);
int check_allocation(int*);
int get_max(int*, int);
int *counting_sort(int**, int);
// void radix_counting_sort(int**, int**, int, int);
// int *radix_sort(int**, int);
int get_repetitions(int**, int);

int main(int argc, char **argv)
{
    srand(time(0));

    int array_size = array_size_growth * atoi(argv[1]); 
    int array_set_size = 2 * array_size;

    int *repetitions_array = calloc(tests, sizeof(int));
    if (check_allocation(repetitions_array) == 1)   {   return 1;   }

    int *array_set = calloc(array_set_size, sizeof(int));
    if (check_allocation(array_set) == 1)   {   return 1;   }

    for (int i = 0; i < tests; i++)
    {
        generate_randnumbers(array_set, array_size, 0);
        generate_randnumbers(array_set, array_size, array_size);
        
        if ((repetitions_array[i] = get_repetitions(&array_set, array_set_size)) == -1)    {   return 1;   }
    }

    free(array_set);
    array_set = NULL;

    FILE *pResultfile = fopen("result.txt", "a");
    if (check_allocation((int*)pResultfile) == 1)     {   return 1;   }

    for (int i = 0; i < tests; i++)
    {
        fprintf(pResultfile, "%d ", repetitions_array[i]);
    }
    fprintf(pResultfile, "\n");
    fclose(pResultfile);

    printf("\rGenerating results...(step number %d)", atoi(argv[1]));

    return 0;
}

// void radix_counting_sort(int **pArray_to_sort, int **sorted_array, int array_size, int digit_index)
// {
//     int count[10] = {0};

//     for (int i = 0; i < array_size; i++)
//     {
//         ++count[((*pArray_to_sort)[i] / digit_index) % 10];
//     }
    
//     for (int i = 1; i < 10; i++)
//     {
//         count[i] = count[i] + count[i - 1];
//     }
    
//     for (int i = array_size - 1; i >= 0; i--)
//     {
//         (*sorted_array)[--count[((*pArray_to_sort)[i] / digit_index) % 10]] = (*pArray_to_sort)[i];
//     }
    
//     int *temp = *sorted_array;
//     *sorted_array = *pArray_to_sort;
//     *pArray_to_sort = temp;

//     temp = NULL;
// }

// int *radix_sort(int **pArray_to_sort, int array_size)
// {
//     int max = get_max(*pArray_to_sort, array_size);
//     int *sorted_array = calloc(array_size, sizeof(int));
//     if (check_allocation(sorted_array) == 1)    {   return NULL;  }

//     for (int digit_index = 1; (max / digit_index) > 0; digit_index *= 10)
//     {
//         radix_counting_sort(pArray_to_sort, &sorted_array, array_size, digit_index);
//     }

//     free(sorted_array);
//     sorted_array = NULL;
//     return *pArray_to_sort;
// }

void generate_randnumbers(int *pArray, int array_size, int index)
{
    for (int i = 0; i < array_size; i++)
    {
        pArray[i + index] = rand() % INT_MAX + 1;
    }
}

int check_allocation(int *pArray)
{
    if (pArray == NULL)
    {
        perror("Error during allocation");
        return 1;
    }
    
    return 0;
}

int get_max(int *pArray, int array_size)
{
    int max_number = 0;
    for (int i = 0; i < array_size; i++)
    {
        if (pArray[i] > max_number)
        {
            max_number = pArray[i];
        }
    }

    return max_number;
}

int *counting_sort(int **pArray_to_sort, int array_size)
{
    int max_number = get_max(*pArray_to_sort, array_size);

    int *sorted_array = calloc(array_size, sizeof(int));
    int *count = calloc(max_number + 1, sizeof(int));

    if (check_allocation(count) == 1 || check_allocation(sorted_array) == 1)    {   return NULL;    }

    for (int i = 0; i < array_size; i++)
    {
        count[(*pArray_to_sort)[i]]++;
    }

    for (int i = 1; i <= max_number; i++)
    {
        count[i] = count[i] + count[i - 1];
    }

    for (int i = array_size - 1; i >= 0; i--)
    {
        sorted_array[--count[(*pArray_to_sort)[i]]] = (*pArray_to_sort)[i];
    }

    free((*pArray_to_sort));
    (*pArray_to_sort) = NULL;
    free(count);
    count = NULL;

    return sorted_array;
}

int get_repetitions(int **array_set, int array_set_size)
{
    *array_set = counting_sort(array_set, array_set_size);
    if (*array_set == NULL)  {   return -1;   }

    // *array_set = radix_sort(array_set, array_set_size);
    // if (*array_set == NULL) {   return -1;  }

    int repetitions = 0;
    for (int i = 0; i < array_set_size; i++)
    {
        if ((*array_set)[i] == (*array_set)[i + 1])
        {
            repetitions++;
        }
    }

    return repetitions;
}