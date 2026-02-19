#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void matrix1p(int rows1, int columns, int matrix1[rows1][columns])
{
    for(int i = 0; i < rows1; i++){
        for(int j = 0; j < columns; j++){
            printf("%2d ", matrix1[i][j]);
        }
        printf("\n");
    }
}

int main(){

    char answer[10];
    int rows1;
    int columns;
    int columns1;

    printf("Enter dimensions for a matrix -\n");

    printf("Rows:");
    scanf("%d", &rows1);
    printf("Columns:");
    scanf("%d", &columns);

    int matrix1[rows1][columns];

    printf("Enter the elements of the matrix\n");
    for(int i = 0; i < rows1; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            scanf("%d", &matrix1[i][j]);
        }
    }

    printf("Here is your matrix --\n");
    matrix1p(rows1, columns, matrix1);

    printf("\nDo you want to mutiply with another matrix or exit?(type 'x' or 'exit') ");
    scanf("%s", &answer);

    if(strcmp(answer, "x") == 0)
    {
        printf("Enter dimensions to another matrix\n");
        printf("Rows: %d\n", columns);
        printf("Columns: ");
        scanf("%d", &columns1);

        int matrix2[columns][columns1];
        int matrixr[rows1][columns1];

        printf("Enter the elements of the matrix\n");
        for(int i = 0; i < columns; i++)
        {
            for(int j = 0; j < columns1; j++)
            {
            scanf("%d", &matrix2[i][j]);
            }
        }
        printf("\n");

        matrix1p(rows1, columns, matrix1);
        printf("x\n");
        for(int i = 0; i < columns; i++)
        {
            for(int j = 0; j < columns1; j++)
            {
            printf("%2d ", matrix2[i][j]);
            }
            printf("\n");
        }
        printf("=\n");

        int a;
        int b = 0;

        int i = 0;
        int j = 0;
        int k = 0;
        int m = 0;
        int n = 0;

        do
        {   
            a = matrix1[i][j] * matrix2[j][k]; 
            b+=a;
            j++;

            if(k == (columns1 - 1) && j == columns )
            {
                matrixr[m][n] = b;
                n++;

                if(n == columns1) 
                { 
                    m++;
                    n = 0;
                }
                b = 0;
                i++;
                j = 0;
                k = 0;
            }
            else if(j == columns) 
            {
                matrixr[m][n] = b;
                n++;

                if(n == columns1) 
                {
                    m++;
                    n = 0;
                }
                b = 0;
                k++;
                j = 0;
            }

        }while(i < rows1);

        for(int w = 0; w < rows1; w++)
        {
            for(int z = 0; z < columns1; z++)
            {
                printf("%.3d ", matrixr[w][z]);
            }
            printf("\n");
        }
    }
    else{
        printf("Bye.");
    }

    return 0;
}