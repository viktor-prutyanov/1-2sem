/**
*   Problems from 2007-2009 tests
*
*   @date 09.2014 - 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define PROBLEM 0 //Number of problem, set to activate needed part of main.

#define MAX_STR 100000
#define MIN5 1000
#define MAX5 10000
#define MAX_VARIANTS 615

typedef enum result_t { OK = 0, NULL_PTR_PARAM = -1 }; 

/**
*   @return 0 if all is OK and -1 if one of array is nullptr
*   @param size is size of matrix
*   @param in_matrix is matrix to transpose
*   @param out_matrix is transposed matrix
*/
result_t transpose(int size, int **in_matrix, int **out_matrix); ///Problem 1

/**
*   @return 0 if all is OK and -1 if one of array is nullptr
*   @param in_string is source string
*   @param out_string is string without whitespaces
*/
result_t delete_whitespaces(char in_string[], char out_string[]); ///Problem 2

/**
*   @return 0 if all is OK and -1 if one of array is nullptr
*   @param size is size of matrix
*   @param in_matrix is matrix to sum
*   @param out_array is an array of sums
*/
result_t sum_num_on_diag(int size, int **in_matrix, int *out_array); ///Problem 3

/**
*   @return 0 if all is OK and -1 if one of array is nullptr
*   @param in_string is source string
*   @param temp_string is temporary string
*   @param out_string is filtered string
*/
result_t symbol_filter(char *in_string, char *temp_string, char *out_string); ///Problem 4

/**
*   @return 0 if all is OK and -1 if one of array is nullptr 
*   @param sum is sum of signs 
*   @param count is count of this numbers
*   @param out_array is array of numbers
*/
result_t four_sign_numbers(int sum, int *count, int *out_array); ///Problem 5

/** 
*   @return 0 if all is OK and -1 if one of array is nullptr
*   @param m is number of rows
*   @param n is number of columns
*   @param in_matrix is matrix to mirror
*   @param out_matrix is mirrored matrix 
*/
result_t mirror_matrix(int m, int n, int **in_matrix, int **out_matrix); ///Problem 6

/**
*   Function extract prime numbers from array
*   @param in_array is source array of unsigned longints
*   @param out_array is array of unsigned longints with only prime numbers
*   @param n is uint number of elements
*   @param c is uint number of primes
*   @return result_t 0 if all is OK and -1 if one or more of params is nullptr
*/
result_t prime_numbers(unsigned long int *in_array, unsigned long int *out_array, unsigned int n, unsigned int *c); ///Problem 7

/**
*   Function checks is number prime or not.
*   @param n is unsigned long integer number
*   @return integer value 1 if number is prime and 0 if number is composite of -1 if number isn't composite or prime 
*/
int is_prime(unsigned long int n); ///Auxillary function

/**
*   Function determines is char digit or not        
*   @param c is char value
*   @return 1 if digit 0 if not
*/
int is_digit(char c);

/**
*   Function determines is char punctuation sign or not        
*   @param c is char value
*   @return 1 if sign 0 if not
*/
int is_sign(char c);

char to_lower_case(char c);

int main()
{
    #if PROBLEM == 1

        int size = 0;
        scanf ("%d", &size);

        int **in_matrix = (int **)calloc (size, sizeof (*in_matrix));
        int **out_matrix = (int **)calloc (size, sizeof (*out_matrix));

        for (int i = 0; i < size; i++)
        {
            assert(0 <= i && i < size);
            in_matrix[i] = (int *)calloc (size, sizeof (*in_matrix[i]));
            out_matrix[i] = (int *)calloc (size, sizeof (*out_matrix[i]));
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                scanf ("%d", (in_matrix[i] + j));
            }
        }

        transpose (size, in_matrix, out_matrix);

        for (int i = 0; i < size; i++)
        {
            assert (0 <= i && i < size);
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                printf ("%d ", out_matrix[i][j]);
            }
            printf ("\n");
        }

        free(in_matrix);
        in_matrix = nullptr;
        free(out_matrix);
        out_matrix = nullptr;

    #elif PROBLEM == 2

        char *in_string = (char *)calloc (MAX_STR, sizeof(char));
        char *out_string = (char *)calloc (MAX_STR, sizeof(char));

        gets (in_string);
        delete_whitespaces (in_string, out_string);
        printf ("%s\n", out_string);

        free(in_string);
        in_string = nullptr;
        free(out_string);
        out_string = nullptr;

    #elif PROBLEM == 3

        int size = 0;
        scanf ("%d", &size);

        int *out_array = (int*)calloc (2 * size - 1, sizeof (*out_array));

        int **in_matrix = (int **)calloc (size, sizeof (*in_matrix));
        for (int i = 0; i < size; i++)
        {
            assert(0 <= i && i < size);
            in_matrix[i] = (int *)calloc (size, sizeof (*in_matrix[i]));
            for (int j = 0; j < size; j++)
            {
                assert (0 <= j && j < size);
                scanf ("%d", (in_matrix[i] + j));
            }
        }

        sum_num_on_diag(size, in_matrix, out_array);

        for (int i = 0; i < size * 2 - 1; i++)
        {
            printf("%d ", out_array[i]);
        }
        printf("\n");

        free(out_array);
        out_array = nullptr;
        free(in_matrix);
        in_matrix = nullptr;

    #elif PROBLEM == 4

        char *in_string = (char *)calloc (MAX_STR, sizeof(* in_string));
        char *out_string = (char *)calloc (MAX_STR, sizeof(* out_string));
        char *temp_string = (char *)calloc (MAX_STR, sizeof(* temp_string));

        gets (in_string);
        symbol_filter (in_string, temp_string, out_string);
        printf ("%s\n", out_string);

        free(in_string);
        in_string = nullptr;
        free(out_string);
        out_array = nullptr;
        free(temp_string);
        temp_array = nullptr;
       
    #elif PROBLEM == 5

        int sum = 0, count =  0;

        int *out_array = (int *)calloc(MAX_VARIANTS, sizeof(*out_array));

        scanf("%d", &sum);
        printf("\n");

        four_sign_numbers(sum, &count, out_array);

        for (int i = 0; i < count; i++)
        {
            printf("%d ", out_array[i]);
        }
        printf("\n%d\n", count);

        free(out_array);
        out_array = nullptr;

    #elif PROBLEM == 6
        int n = 0, m = 0;
        scanf ("%d  %d", &m , &n);

        int **in_matrix = (int **)calloc (m, sizeof (*in_matrix));
        int **out_matrix = (int **)calloc (m, sizeof (*out_matrix));

        for (int i = 0; i < m; i++)
        {
            assert(0 <= i && i < m);
            in_matrix[i] = (int *)calloc (n, sizeof (*in_matrix[i]));
            out_matrix[i] = (int *)calloc (n, sizeof (*out_matrix[i]));
            for (int j = 0; j < n; j++)
            {
                assert (0 <= j && j < n);
                scanf ("%d", (in_matrix[i] + j));
            }
        }

        mirror_matrix (m, n, in_matrix, out_matrix);

        for (int i = 0; i < m; i++)
        {
            assert (0 <= i && i < m);
            for (int j = 0; j < n; j++)
            {
                assert (0 <= j && j < n);
                printf ("%d ", out_matrix[i][j]);
            }
            printf ("\n");
        }

        free (in_matrix);
        free (out_matrix);
        in_matrix = nullptr;
        out_matrix = nullptr;

    #elif PROBLEM == 7

        unsigned int n = 0, c = 0;
        scanf ("%u", &n);

        long unsigned int *in_array = (long unsigned int *)calloc (n, sizeof(*in_array));
        long unsigned int *out_array = (long unsigned int *)calloc (n, sizeof(*out_array));

        for (int i = 0; i < n; i++)
        {
            assert (0 <= i && i < n);
            scanf ("%u", &in_array[i]);
        }

        prime_numbers (in_array, out_array, n, &c);

        for (int i = 0; i < c; i++)
        {
            assert (0 <= i && i < n);
            printf ("%u ", out_array[i]);
        }

        printf ("\n");

        free(in_array);
        in_array = nullptr;
        free(out_array);
        out_array = nullptr;

    #endif

    #ifdef _DEBUG
        system ("pause");
    #endif

    return 0;
}

result_t mirror_matrix(int m, int n, int **in_matrix, int **out_matrix) ///Problem 6
{
    if (in_matrix == nullptr || out_matrix == nullptr)
    {
        return NULL_PTR_PARAM;
    }

    for (int i = 0; i < m; i++)
    {
        assert (0 <= i && i < m);
        for (int j = 0; j < n; j++)
        {
            assert (0 <= j && j < n);
            assert (0 <= m - i - 1 && m - i - 1 < m);
            assert (0 <= n - j - 1 && n - j - 1< n);
            out_matrix[i][j] = in_matrix[m - i - 1][n - j - 1];
        }
    }
    return OK;
}

result_t four_sign_numbers(int sum, int *count, int *out_array) ///Problem 5
{
    if (count == nullptr || out_array == nullptr)
    {
        return NULL_PTR_PARAM;
    }
    int p = 0;
    for (int i = MIN5; i < MAX5; i++)
    {
        if ((i / 1000) + (i / 100) % 10 + (i / 10) % 10 + i % 10 == sum)
        {
            out_array[p] = i;
            p++;
        }
    }

    *count = p;
    return OK;
}

result_t delete_whitespaces(char *in_string, char *out_string) ///Problem 2
{
    if (in_string == nullptr || out_string == nullptr)
    {
        return NULL_PTR_PARAM;
    }

    int length = strlen (in_string);
    int pos = 0;
    
    for (int i = 0; i < length - 1; i++)
    {
        assert (0 <= i && i < length - 1);
        if (!((in_string[i] == ' ' || in_string[i] == '\t') && (in_string[i + 1] == ' ' || in_string[i + 1] == '\t')))
        {
            assert((i + 1) < length);
            out_string[pos] = in_string[i];
            pos++;
        }
        if (in_string[length - 1] != ' ' && in_string[length - 1] != '\t') 
        {
            out_string[pos] = in_string[length - 1];
        }
    }

    return OK;
}

result_t symbol_filter(char *in_string, char *temp_string, char *out_string) ///Problem 4
{
    if (in_string == nullptr || out_string == nullptr || temp_string == nullptr)
    {
        return NULL_PTR_PARAM;
    }

    delete_whitespaces(in_string, temp_string);
    int length = strlen (temp_string);
    int pos = 0;

    for (int i = 0; i < length; i++)
    {
        assert(0 <= i && i < length);
        if (!is_sign(temp_string[i]))
        {
            if (is_digit(temp_string[i]))
            {
                out_string[pos] = '#';
            }
            else
            {
                out_string[pos] = to_lower_case(temp_string[pos]);
            }
            pos++;
        }
    }

    return OK;
}

result_t sum_num_on_diag(int size, int **in_matrix, int *out_array) ///Problem 3
{
    if (in_matrix == nullptr || out_array == nullptr)
    {
        return OK;
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            assert (0 <= j && j < size);
            assert (0 <= (size - i + j - 1) && (size - i + j - 1) < size);
            out_array[i] += in_matrix[size - i + j - 1][j];
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            assert (0 <= j && j < size);
            assert (0 <= (size - i + j - 1) && (size - i + j - 1) < size);
            out_array[size * 2 - 2 - i] += in_matrix[j][size - i + j - 1];
        }
    }

    out_array[size - 1] /= 2; 

    return OK;
}

result_t transpose(int size, int **in_matrix, int **out_matrix) ///Problem 1
{
    if (in_matrix == nullptr || out_matrix == nullptr)
    {
        return NULL_PTR_PARAM;
    }

    for (int i = 0; i < size; i++)
    {
        assert (0 <= i && i < size);
        for (int j = 0; j < size; j++)
        {
            assert (0 <= j && j < size);
            *(out_matrix[i] + j) = *(in_matrix[j] + i); 
        }
    }
    return OK;
} 

result_t prime_numbers(unsigned long int *in_array, unsigned long int *out_array, unsigned int n, unsigned int *c) ///Problem 7
{
    if (in_array == nullptr || out_array == nullptr)
    {
        return NULL_PTR_PARAM;
    }

    int j = 0;

    for (int i = 0; i < n; i++)
    {
        assert (0 <= i && i < n);
        if (is_prime (in_array[i]) == 1)
        {
            out_array[j] = in_array[i];
            j++;
        }
    }

    *c = j;

    return OK;
}

int is_prime(unsigned long int n)
{
    if (n == 0 || n == 1)
    {
        return -1;
    }
    else if (n % 2 == 0)
    {
        return 0;
    }
    else
    {
        for (int i = 3; i < floor (sqrt ((double)n) + 1); i += 2)
        {
            if (n % i == 0)
            {
                return 0;
            }
        }
        return 1;
    }
}

int is_digit(char c)
{
    if (48 <= c && c <= 57)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_sign(char c)
{
    if (c == '?'|| c == '!'|| c == '.'|| c == ',' || c == ';' || c == '-' || c == '(' || c == ')' || c == ':')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char to_lower_case(char c)
{
    if (65 <= c && c <= 90)
    {
        return c + 32;
    }
    else 
    {
        return c;
    }
}
