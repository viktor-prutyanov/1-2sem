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
int str_ptr_cmp(const void *a, const void *b);

/**
    @brief Function that compares two strings presented by pointers in reversed order

    @param a is first pointer to string
    @param b is second pointer to string

    @return -1 if a < b, 0 if a = b, 1 if a > b  
*/
int str_ptr_cmp_rev(const void *a, const void *b);

/**
    @brief Function that gets length of file

    @param file is pointer to file

    @return length of file in bytes 
*/
unsigned long int file_length(FILE *file);

/**
    @brief Function that outputs array of pointes to string to file

    @param lines is number of lines
    @param text is array of pointers
    @param file is output file

    @return 0 if everything is OK, 1 if error exist
*/
int str_array_to_file(unsigned long int lines, char **text, FILE *file);

/**
    @brief Function that writes string divided by null to array of pointers to string

    @param text is array of pointers
    @param buf is output file
    @param len is length of buf
    @param lines is number of lines

    @return amount of lines that was writed
*/
char *read_file_to_string(char filename[], long unsigned int *length, long unsigned int *len);

/**
    @brief Function that reads file to array of strings
    @warning length and read_length can be inequal, for example, if you use Windows file endings (CR LF).

    @param filename is address of file
    @param read_length is a pointer where will be write amount of symbols readed to string
    @param length is a pointer where will be write amount of strings that contains in file
    @param lines is a pointer where will be write amount of lines
    @param buf is a address of text in memory

    @return array of pointers to strings 
*/
char **read_strings_from_file(char filename[], long unsigned int *length, long unsigned int *len, long unsigned int *lines, char *buf);

/**
    @brief Function that writes array of strings into file

    @param filename is name of file
    @param lines is amount of lines
    @param text is array of string

    @return 0 if everything is OK, 1 if something went wrong
*/
int write_strings_to_file(char filename[], long unsigned int lines, char **text);

int main()
{
    #ifdef _DEBUG
        LARGE_INTEGER time0 = { }, time1 = { };
        QueryPerformanceCounter (&time0);
    #endif

    printf ("In which order do you want to sort Eugene Onegin? Enter (1)direct or (0)reverse\n");
    int order = -1;
    scanf ("%d", &order);
    if (order != 1 && order != 0) 
    {
        printf("Incorrect order.\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    printf ("Your choice is %s order.\n", order ? "direct" : "reverse");

    long unsigned int read_length = 0, length = 0, lines = 0;
    char **text;
    char *buf = nullptr;
    text = read_strings_from_file("..\\eugene_onegin.txt", &length, &read_length, &lines, buf);
    if (text == nullptr) 
    {
        printf ("Reading failed.\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    printf ("There are %u symbols in file. Read %u symbols. Read %u lines.\n", length, read_length, lines);

    qsort (text, lines, sizeof(*text), order ? str_ptr_cmp : str_ptr_cmp_rev);

    if (write_strings_to_file("..\\eugene_onegin_sorted.txt", lines, text)) 
    {
        printf ("Writing failed.\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    free (text);
    text = nullptr;
    free (buf);
    buf = nullptr;
    
    #ifdef _DEBUG
        QueryPerformanceCounter (&time1);
        printf ("QueryPerformanceCounter: %lld\n", time1.QuadPart - time0.QuadPart);
    #endif

    #ifdef _DEBUG
        system ("pause");
    #endif
  
    return 0;
}

int str_ptr_cmp(const void *a, const void *b)
{
    return strcmp (*(char **)a, *(char **)b);
}

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
        assert(0 <= len1 - 1 - i && len1 - 1 - i < len1);
        assert(0 <= len2 - 1 - i && len2 - 1 - i < len2);
        int diff = s1[len1 - 1 - i] - s2[len2 - 1 - i];
        if (diff) return diff;
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

unsigned long int file_length(FILE *file)
{
    if (file == nullptr) return 0;
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    rewind (file);
    return length;
}

int str_array_to_file(unsigned long int lines, char **text, FILE *file)
{
    if ((file == nullptr) || (text == nullptr) || (file == nullptr)) return 1;
    for (int i = 0; i < lines; i++)
    {
        assert (0 <= i && i < lines);
        if (*text[i])
        {
            #ifdef _DEBUG
                fprintf (file, "|%s|\n", text[i]);
            #endif
            fprintf (file, "%s\n", text[i]);
        }
    }
    return 0;
}

char *read_file_to_string(char filename[], long unsigned int *length, long unsigned int *read_length)
{
    if (filename == nullptr) return nullptr;
    FILE *in_file = fopen (filename, "r");
    if (in_file == nullptr) return nullptr;
    *length = file_length (in_file); //unsigned long int length = _filelength(_fileno(file)) + 1;
    char *buf = (char *)calloc (*length + 1, sizeof(char));
    if (buf == nullptr) return nullptr;
    *read_length = fread (buf, sizeof(char), *length, in_file);
    fclose (in_file);
    return buf;
}

char **read_strings_from_file(char filename[], long unsigned int *length, long unsigned int *read_length, long unsigned int *lines, char *buf)
{
    if ((filename == nullptr) || (length == nullptr) || (read_length == nullptr) || (lines == nullptr)) return nullptr;

    buf = read_file_to_string (filename, length, read_length);

    for (int i = 0; i < *read_length + 1; i++)
    {
        assert (0 <= i && i < *read_length + 1);
        if (buf[i] == '\n') 
        {
            buf[i] = '\0';
            (*lines)++;
        }
        else if (!buf[i])
        {
            (*lines)++;
        }
    }

    char **text = (char**)calloc (*lines + 1, sizeof(*text));

    text[0] = &(buf[0]);

    long unsigned int pos = 1;

    for (int i = 0; i < *read_length; i++)
    {
        assert (0 <= i && i < *read_length + 1);
        assert (0 <= i + 1 && i + 1 < *read_length + 1);
        if (buf[i] == '\0')
        {
            assert(0 <= pos && pos < *lines + 1);
            text[pos] = &(buf[i + 1]);
            pos++;
        }
    }

    if (*lines == pos)
    {
        return text;
    }
    else
    {
        return nullptr;
    }
}

int write_strings_to_file(char filename[], long unsigned int lines, char **text)
{
    if ((filename == nullptr) || (text == nullptr)) return 1;
    FILE *out_file = fopen (filename, "w");
    if (out_file == nullptr) return 1;
    if (str_array_to_file (lines, text, out_file)) return 1;
    fclose (out_file);
    out_file = nullptr;
    return 0;
}