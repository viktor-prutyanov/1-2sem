/**
*   Assemler
*
*   @file stack.h
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
    @brief Creates array of addresses that specified by marks

    @param in_file is file with source codes
    @param[out] marks is array of addresses

    @return amount of really written commands 
*/
int mark_to_jump(FILE *in_file, double *marks);

/**
    @brief Writes command with number com_num to output

    @param com_num number of command
    @param out_file is output file

    @return amount of marks
*/
size_t write_command(double com_num, FILE *out_file);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf ("Usage:\n\tasm <input file> <output file>\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    
    FILE *in_file  = fopen(argv[1], "r");
    FILE *out_file = fopen(argv[2], "wb");
    if (in_file == nullptr || out_file == nullptr) 
    {
        printf ("One or more incorrect files.\n");

        fcloseall();
        in_file = nullptr;
        out_file = nullptr;

        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    printf("Input is %s, output is %s\n", argv[1], argv[2]);

    bool next_is_com = true;
    char com[5] = {0};
    double val = 0;
    double marks[10] = {0};
    int count = 0;
    int success = 0;

    mark_to_jump(in_file, marks);

    while (!feof (in_file))
    {
        if (next_is_com)
        {
            fscanf (in_file, "%s", &com);
            if ((com[0] == ':') && (count != (int)(marks[com[1] - 48])))
            {
                fwrite ((void *)(marks + (com[1] - 48)), sizeof (double), 1, out_file);
            }
            else if (com[0] == ':')
            {
                write_command(4, out_file);
            }
            #define DEF_CMD(cmd, num, code, name, args)                                       \
            else if (strcmp (com, name) == 0)                                                 \
            {                                                                                 \
                val = num;                                                                    \
                if (num == 1)                                                                 \
                {                                                                             \
                    count++;                                                                  \
                    success = fscanf (in_file, "%lf", &val);                                  \
                    if (success)                                                              \
                    {                                                                         \
                        write_command(1, out_file);                                           \
                        write_command(val, out_file);                                         \
                    }                                                                         \
                    else                                                                      \
                    {                                                                         \
                        fscanf (in_file, "%s", &com);                                         \
                        if (strcmp(com, "ax") == 0)                                           \
                        {                                                                     \
                            write_command(17, out_file);                                      \
                        }                                                                     \
                    }                                                                         \
                }                                                                             \
                else if (num == 2)                                                            \
                {                                                                             \
                    count++;                                                                  \
                    fscanf (in_file, "%s", &com);                                             \
                    if (strcmp(com, "ax") == 0)                                               \
                    {                                                                         \
                        write_command(18, out_file);                                          \
                    }                                                                         \
                }                                                                             \
                else                                                                          \
                {                                                                             \
                    write_command(val, out_file);                                             \
                    if ((args == 1) && !((num >= 10) && (num <= 16)))  next_is_com = false;   \
                }                                                                             \
            }
            #include "..\include\commands.h"
            #undef DEF_CMD
            else
            {
                printf("%s (#%d): invalid or unsupported command\n", com, count);
                break;
            }
        }
        else
        {
            success = fscanf (in_file, "%lf", &val);
            printf ("%lf", val);
            write_command(val, out_file);
            next_is_com = true;
        }
        count++;
    }

    
    fcloseall();
    in_file = nullptr;
    out_file = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif
    return 0;
}


int mark_to_jump(FILE *in_file, double *marks)
{
    char val[5];
    char prev_val_zero_symb = '0';
    int mark_count = 0;
    int count = 0;

    while (!feof (in_file))
    {
        fscanf (in_file, "%s", &val);
        if ((val[0] == ':') && (prev_val_zero_symb == 'j'))
        {
            mark_count++;
        }
        else if ((val[0] == ':') && (prev_val_zero_symb != 'j'))
        {
            marks[val[1] - 48] = count;
        }
        prev_val_zero_symb = val[0];
        count++;
    }

    fseek(in_file, 0, SEEK_SET);
    return mark_count;
}

size_t write_command(double com_num, FILE *out_file)
{
    return fwrite ((void *)(&com_num), sizeof (double), 1, out_file);
}