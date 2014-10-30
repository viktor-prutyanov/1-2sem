/**
*   VP1 Assemler
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

    @return amount of marks
*/
int mark_to_jump(FILE *in_file, double *marks);

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

    bool next_is_command = true;
    char command[5] = {0};
    double val = 0;
    double marks[10] = {0};
    int count = 0;
    double nop = 4;

    mark_to_jump(in_file, marks);

    while (!feof (in_file))
    {
        if (next_is_command)
        {
            fscanf (in_file, "%s", &command);
            if ((command[0] == ':') && (count != (int)(marks[command[1] - 48])))
            {
                fwrite ((void *)(marks + (command[1] - 48)), sizeof (double), 1, out_file);
            }
            else if (command[0] == ':')
            {
                fwrite ((void *)(&nop), sizeof (double), 1, out_file);
            }
            #define DEF_CMD(cmd, num, code, name, args)                                                 \
            else if (strcmp (command, name) == 0)                                                       \
            {                                                                                           \
                val = num;                                                                              \
                fwrite ((void *)(&val), sizeof (double), 1, out_file);                                  \
                if ((args == 1) && !((num >= 10) && (num <= 16)))  next_is_command = false;             \
            }
            #include "..\include\commands.h"
            #undef DEF_CMD
            else
            {
                printf("%s (#%d): invalid or unsupported command\n", command, count);
                break;
            }
        }
        else
        {
            fscanf (in_file, "%lf", &val);
            fwrite ((void *)(&val), sizeof (double), 1, out_file); 
            next_is_command = true;
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
