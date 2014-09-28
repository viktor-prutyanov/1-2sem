#include <stdio.h>0
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define PROBLEM 0 //Number of problem (1-6), set to activate needed part of main.

#define MAX_STR 100000
#define MAX_VARIANTS 615

int** mirror_matrix(int m, int n, int **matrix) ///Problem 1
{
/** @return horizontal and vertical mirrored matrix
*   @param m is number of rows
*   @param n is number of columns
*   @param matrix is matrix to mirror
*/
    int **mirrored_matrix = (int **)calloc (m, sizeof (*mirrored_matrix));
    for (int i = 0; i < m; i++)
    {
        assert (0 <= i && i < m);
        mirrored_matrix[i] = (int *)calloc (n, sizeof (*mirrored_matrix[i]));
        for (int j = 0; j < n; j++)
        {
            assert (0 <= j && j < n);
            assert (0 <= m - i - 1 && m - i - 1 < m);
            assert (0 <= n - j - 1 && n - j - 1< n);
            mirrored_matrix[i][j] = matrix[m - i - 1][n - j - 1];
        }
    }
    return mirrored_matrix;
} 

int* four_sign_numbers(int sum, int * count) ///Problem 5
{
/**
*    @return array of 4-sign numbers, thats sum of signs is sum
*    @param sum is sum of signs 
*    @param count is count of this numbers
*/
    int *temp = (int *)calloc(MAX_VARIANTS, sizeof(*temp));
    int p = 0;
    for (int i = 1000; i < 10000; i++)
    {
        if ((i / 1000) + (i / 100) % 10 + (i / 10) % 10 + i % 10 == sum)
        {
            temp[p] = i;
            p++;
        }
    }

    *count = p;
    return temp;
}

char* delete_whitespaces(char source[]) ///Problem 2
{
/**
*    @return string without unnecessary whitespace
*    @param source is source string
*/
    int length = strlen (source);
    int pos = 0;
    char *str = (char *)calloc (length, sizeof(*str));

    for (int i = 0; i < length - 1; i++)
    {
        assert (0 <= i && i < length - 1);
        if (!((source[i] == ' ' || source[i] == '\t') && (source[i + 1] == ' ' || source[i + 1] == '\t')))
        {
            assert((i + 1) < length);
            str[pos] = source[i];
            pos++;
        }
        if (source[length - 1] != ' ' && source[length - 1] != '\t') str[pos] = source[length - 1];
    }
    return str;
}

char* symbol_filter(char source[]) ///Problem 4
{
/**
*    @return string without unnecessary spaces and tabs, humbers changed to #, without upper case and without punctuation signs.
*    @param source is source string
*/
    char *temp_str = delete_whitespaces (source);
    int length = strlen (temp_str);
    char *result_str = (char *)calloc (length, sizeof(*result_str));
    int pos = 0;

    for (int i = 0; i < length; i++)
    {
        assert(0 <= i && i < length);
        if (!(temp_str[i] == '?'|| temp_str[i] == '!'|| temp_str[i] == '.'|| temp_str[i] == ',' || temp_str[i] == ';' || temp_str[i] == '-' || temp_str[i] == '(' || temp_str[i] == ')'))
        {
            if (48 <= temp_str[i] && temp_str[i] <= 57)
            {
                result_str[pos] = '#';
            }
            else if (65 <= temp_str[i] && temp_str[i] <= 90)
            {
                result_str[pos] = temp_str[i] + 32;
            }
            else
            {
                result_str[pos] = temp_str[i];
            }
            pos++;
        }
    }

    free(temp_str);
    temp_str = nullptr;
    return result_str;
}

int* sum_num_on_diag(int size, int **matrix) ///Problem 3
{
/**
*    @return array of sums by diagonal
*    @param size is size of matrix
*    @param matrix is matrix to sum
*/
    
    int *sums = (int*)calloc (2 * size - 1, sizeof (*sums));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            assert (0 <= j && j < size);
            assert (0 <= (size - i + j - 1) && (size - i + j - 1) < size);
            sums[i] += matrix[size - i + j - 1][j];
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            assert (0 <= j && j < size);
            assert (0 <= (size - i + j - 1) && (size - i + j - 1) < size);
            sums[size * 2 - 2 - i] += matrix[j][size - i + j - 1];
        }
    }

    sums[size - 1] /= 2; 

    return sums;
}

int** transpose(int size, int **matrix) ///Problem 1
{
/**
*    @return transposed matrix
*    @param size is size of matrix
*    @param matrix is matrix to transpose
*/
    int **transposed_matrix = (int **)calloc (size, sizeof (*transposed_matrix));
    for (int i = 0; i < size; i++)
    {
        assert (0 <= i && i < size);
        transposed_matrix[i] = (int *)calloc (size, sizeof (*transposed_matrix[i]));
        for (int j = 0; j < size; j++)
        {
            assert (0 <= j && j < size);
            *(transposed_matrix[i] + j) = *(matrix[j] + i); 
        }
    }
    return transposed_matrix;
} 

int main()
{
    #if PROBLEM == 1

        int size = 0;
        scanf ("%d", &size);

        //Input square matrix
        int **matrix = (int **)calloc (size, sizeof (*matrix));
        for (int i = 0; i < size; i++)
        {
            assert(0 <= i && i < size);
            matrix[i] = (int *)calloc (size, sizeof (*matrix[i]));
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                scanf ("%d", (matrix[i] + j));
            }
        }

        int **transposed_matrix = transpose (size, matrix);

        //Output matrix
        for (int i = 0; i < size; i++)
        {
            assert (0 <= i && i < size);
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                printf ("%d ", transposed_matrix[i][j]);
            }
            printf ("\n");
        }

        free(transposed_matrix);
        transposed_matrix = nullptr;
        free(matrix);
        matrix = nullptr;

    #elif PROBLEM == 2

        char source_str[MAX_STR];
        gets (source_str);
        printf ("%s\n", delete_whitespaces (source_str));

    #elif PROBLEM == 3

        int size = 0;
        scanf ("%d", &size);

        //Input matrix
        int **matrix = (int **)calloc (size, sizeof (*matrix));
        for (int i = 0; i < size; i++)
        {
            assert(0 <= i && i < size);
            matrix[i] = (int *)calloc (size, sizeof (*matrix[i]));
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                scanf ("%d", (matrix[i] + j));
            }
        }

        int *sums = (int*)calloc (2 * size - 1, sizeof (*sums));

        sums = sum_num_on_diag(size, matrix);

        for (int i = 0; i < size * 2 - 1; i++)
        {
            printf("%d ", sums[i]);
        }

        free(sums);
        sums = nullptr;
        printf("\n");

    #elif PROBLEM == 4

        char *source_str = (char *)calloc (MAX_STR, sizeof(* source_str));
        gets (source_str);
        printf ("%s\n", symbol_filter (source_str));
       
    #elif PROBLEM == 5

        int sum = 0, count = 0;

        scanf("%d", &sum);
        printf("\n");

        int *numbers;
        numbers = four_sign_numbers(sum, &count);

        for (int i = 0; i < count; i++)
        {
            printf("%d ", numbers[i]);
        }
        printf("\n%d\n", count);
        free(numbers);
        numbers = nullptr;

    #elif PROBLEM == 6
        int n = 0, m = 0;
        scanf ("%d  %d", &m , &n);

        //Input matrix
        int **matrix = (int **)calloc (m, sizeof (*matrix));
        for (int i = 0; i < m; i++)
        {
            assert(0 <= i && i < m);
            matrix[i] = (int *)calloc (n, sizeof (*matrix[i]));
            for (int j = 0; j < n; j++)
            {
                assert (0 <= j && j < n);
                scanf ("%d", (matrix[i] + j));
            }
        }

        int **mirrored_matrix = mirror_matrix (m, n, matrix);

        //Output matrix
        for (int i = 0; i < m; i++)
        {
            assert (0 <= i && i < m);
            for (int j = 0; j < n; j++)
            {
                assert (0 <= j && j < n);
                printf ("%d ", mirrored_matrix[i][j]);
            }
            printf ("\n");
        }

        free (mirrored_matrix);
        free (matrix);
        matrix = nullptr;
        mirrored_matrix = nullptr;

    #endif

    system ("pause");
    return 0;
}