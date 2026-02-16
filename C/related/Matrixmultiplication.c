#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define INPUTFILE_MAXSIZE 250
#define MAX_BUFFER 10
#define MAX_NUMBER 99999
#define MAX_DIGITS 5

int inputf(char*, int);
int inputf_number(bool);
void inputf_number_reset(char*, bool*);
bool inputf_number_check(char*, int, int, bool*);
int matrix_inputf_number(int, int, int**);
void matrix_printf(int, int, int**);
int **matrix_init(int, int);
void matrix_product(int, int, int, int**, int**, int**);
void matrix_free(int, int**);

int main()
{
    int i, j, k = 0;

    int **Matrix_1 = NULL;
    int **Matrix_2 = NULL;
    int **Matrix_result = NULL;

    printf("Enter dimensions for matrix 1 -\n");
    printf("Rows: ");
    i = inputf_number(false);
    printf("Columns: ");
    j = inputf_number(false);

    printf("Enter columns for matrix 2(the rows of the second matrix are equal to the columns of the first matrix) - \n");
    printf("Columns: ");
    k = inputf_number(false);

    Matrix_1 = matrix_init(i, j);
    Matrix_2 = matrix_init(j, k);
    Matrix_result = matrix_init(i, k);

    printf("\nNow, enter the values\n");
    printf("to Matrix 1:\n");
    matrix_inputf_number(i, j, Matrix_1);
    printf("to Matrix 2:\n");
    matrix_inputf_number(j, k, Matrix_2);

    printf("\nHere is Matrix 1\n");
    matrix_printf(i, j , Matrix_1);
    printf("Here is Matrix 2\n");
    matrix_printf(j, k, Matrix_2);

    matrix_product(i, j, k, Matrix_1, Matrix_2, Matrix_result);

    printf("\nThe result matrix is\n");
    matrix_printf(i, k, Matrix_result);

    matrix_free(i, Matrix_1);
    matrix_free(j, Matrix_2);
    matrix_free(i, Matrix_result);

    Sleep(10000);
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
    tempstr[strcspn(tempstr, "\n")] = '\0';
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

int matrix_inputf_number(int rows, int columns, int **Matrix)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            printf("[%d][%d] = ", i, j);
            Matrix[i][j] = inputf_number(true);
        }
    }
}

void matrix_printf(int rows, int columns, int **Matrix)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            printf("%d   ", Matrix[i][j]);
        }
        printf("\n");
    }
}

int **matrix_init(int rows, int columns)
{
    int **temp = calloc(rows, sizeof(int*));
    if(temp == NULL)
    {
        perror("Fail during allocation");
        return NULL;
    }

    for(int i = 0; i < rows; i++)
    {
        temp[i] = calloc(columns, sizeof(int));
        if(temp[i] == NULL)
        {
            perror("Fail during allocation");
            return NULL;
        }
    }

    return temp;
}

void matrix_product(int rows, int columns_rows, int columns, int **Matrix1, int **Matrix2, int **Matrixresult)
{
    for(int i = 0; i < rows; i++)
    {
        for(int k = 0; k < columns; k++)
        {
            for(int j = 0; j < columns_rows; j++)
            {
                Matrixresult[i][k] += Matrix1[i][j] * Matrix2[j][k];
            }
        }
    }
}

void matrix_free(int rows, int **Matrix)
{
    for(int i = 0; i < rows; i++)
    {
        free(Matrix[i]);
        Matrix[i] = NULL;
    }

    free(Matrix);
    Matrix = NULL;
}