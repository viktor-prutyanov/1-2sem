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
    FILE *in_file = fopen ("..\\eugene_onegin.txt", "r");
    assert (in_file != nullptr);
    unsigned long int length = file_length(in_file); //unsigned long int length = _filelength(_fileno(file)) + 1;
    char *buf = (char *)calloc (length, sizeof(char));
    assert (buf != nullptr);
    unsigned long int len = fread (buf, sizeof(char), length, in_file);
    fclose (in_file);

    printf ("File length is %u symbols.\nReaded %u symbols.\n", length, len);
    
    unsigned long int lines = change_n_to_0 (buf, len); 

    printf ("Readed %u lines.\n", lines);

    char **text = (char**)calloc (lines + 1, sizeof(*text));

    text[0] = &(buf[0]);

    unsigned long int pos = 1;

    for (int i = 0; i < len - 1; i++)
    {
        assert (0 <= i && i < len);
        assert (0 <= i + 1 && i + 1 < len);
        if (buf[i] == '\0')
        {
            assert(0 <= pos && pos < lines + 1);
            text[pos] = &(buf[i + 1]);
            pos++;
        }
    }

    printf ("Pointers created for %u strings.\n", pos);

    qsort (text, lines, sizeof(char *), str_ptr_cmp); 

    FILE *out_file = fopen ("..\\eugene_onegin_sorted.txt", "w");
    str_array_to_file(lines, text, out_file);
    fclose (out_file);
    out_file = nullptr;

    free (buf);
    buf = nullptr;
    free (text);
    text = nullptr;

    #ifdef _DEBUG
        system("pause");
    #endif

    return 0;
}