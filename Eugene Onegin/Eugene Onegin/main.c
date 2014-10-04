/**
*   Program for creation encyclopedia of russian XIX century life (A.S.Pushkin "Eugene Onegin")
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <assert.h>
#include <string.h>

#ifdef _DEBUG
    #include <Windows.h>
#endif

/**
    @brief Function that compares two strings presented by pointers

    @param a is first pointer to string
    @param b is second pointer to string

    @return -1 if a < b, 0 if a = b, 1 if a > b  
*/
int str_ptr_cmp(const void *a, const void *b)
{
    return strcmp (*(char **)a, *(char **)b);
}


/**
    @brief Function that compares two strings presented by pointers in reversed order

    @param a is first pointer to string
    @param b is second pointer to string

    @return -1 if a < b, 0 if a = b, 1 if a > b  
*/
int str_ptr_cmp_rev(const void *a, const void *b)
{
    char *s1 = *(char **)a; 
    char *s2 = *(char **)b;

    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len = 0;

    if (len1 >  len2)
    {
        len = len2;
    }
    else
    {
        len = len1;
    }

    for (int i = 0; i < len; i++)
    {
        if (s1[len1 - 1 - i] > s2[len2 - 1 -i])
        {
            return 1;
        }
        else if (s1[len1 - 1 - i] < s2[len2 - 1 -i])
        {
            return -1;
        }
    }

    if (len1 > len2)
    {
        return 1;
    }
    else if (len1 < len2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
    @brief Function that gets length of file

    @param file is pointer to file

    @return length of file in bytes 
*/
unsigned long int file_length(FILE *file)
{
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    rewind (file);
    return length;
}

/**
    @brief Function changes symols of newline to 0

    @param str is string to change

    @return len is length of string
*/
unsigned long int change_n_to_0(char *str, unsigned long int len)
{
    unsigned long int lines = 0;

    for (int i = 0; i < len; i++)
    {
        assert (0 <= i && i < len);
        if (str[i] == '\n') 
        {
            str[i] = '\0';
            lines++;
        }
    }
    return lines;
}

/**
    @brief Function that outputs array of pointes to string to file

    @param lines is number of lines
    @param text is array of pointers
    @param file is output file
*/
void str_array_to_file(unsigned long int lines, char **text, FILE *file)
{
    for (int i = 0; i < lines; i++)
    {
        assert (0 <= i && i < lines);
        if (*text[i])
        {
            fprintf (file, "%s\n", text[i]);
        }
    }
}

int main()
{
    #ifdef _DEBUG
        LARGE_INTEGER time0 = { }, time1 = { };
        QueryPerformanceCounter(&time0);
    #endif

    FILE *in_file = fopen ("..\\eugene_onegin.txt", "r");

    assert (in_file != nullptr);
    unsigned long int length = file_length(in_file); //unsigned long int length = _filelength(_fileno(file)) + 1;
    char *buf = (char *)calloc (length + 1, sizeof(char));
    assert (buf != nullptr);
    unsigned long int len = fread (buf, sizeof(char), length, in_file);
    fclose (in_file);

    printf("In which order do you want to sort Eugene Onegin? Enter 1(direct) or -1(reverse):\n");
    int order = 0;
    scanf("%d", &order);
    assert(order == 1 || order == -1);
    order == 1 ? printf("You choice is direct order.\n") : printf("You choice is reverse order.\n");

    printf ("File length is %u symbols.\nReaded %u symbols.\n", length, len);
    
    buf[len] = '\n';

    unsigned long int lines = change_n_to_0 (buf, len + 1); 

    printf ("Readed %u lines.\n", lines);

    char **text = (char**)calloc (lines + 1, sizeof(*text));

    text[0] = &(buf[0]);

    unsigned long int pos = 1;

    for (int i = 0; i < len; i++)
    {
        assert (0 <= i && i < len + 1);
        assert (0 <= i + 1 && i + 1 < len + 1);
        if (buf[i] == '\0')
        {
            assert(0 <= pos && pos < lines + 1);
            text[pos] = &(buf[i + 1]);
            pos++;
        }
    }

    printf ("Pointers created for %u strings.\n", pos);

    order == 1 ? qsort (text, lines, sizeof(char *), str_ptr_cmp) : qsort (text, lines, sizeof(char *), str_ptr_cmp_rev); 

    FILE *out_file = fopen ("..\\eugene_onegin_sorted.txt", "w");
    str_array_to_file(lines, text, out_file);
    fclose (out_file);
    out_file = nullptr;

    free (buf);
    buf = nullptr;
    free (text);
    text = nullptr;
    
    #ifdef _DEBUG
        QueryPerformanceCounter(&time1);
        printf("%d\n", time1.QuadPart - time0.QuadPart);
    #endif

    #ifdef _DEBUG
        system("pause");
    #endif
  
    return 0;
}