/**
*   Disassmebler
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

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf ("Usage:\n\tdisasm <input file> <output file>\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    
    FILE *in_file  = fopen(argv[1], "rb");
    FILE *out_file = fopen(argv[2], "w+");

    if (in_file == nullptr || out_file == nullptr) 
    {
        printf ("One or more incorrect files.\n");

        fcloseall ();
        in_file = nullptr;
        out_file = nullptr;

        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    printf ("Input is %s, output is %s\n", argv[1], argv[2]);

    bool next_is_command = true;
    double val = -1;
    int count = 0;

    while (!feof (in_file))
    {
        fread ((void *)(&val), sizeof (double), 1, in_file);
        if (next_is_command)
        {
            if (val == -1) 
            {
                break;
            }
            #define DEF_CMD(cmd, num, code, name, args)  \
            else if (num == val)                         \
            {                                            \
                if (args == 1)                           \
                {                                        \
                    fprintf (out_file, "%s ", name);     \
                    next_is_command = false;             \
                }                                        \
                else                                     \
                {                                        \
                    fprintf (out_file, "%s\n", name);    \
                }                                        \
            }
            #include "..\include\commands.h"
            #undef DEF_CMD
            else
            {
                printf("%lf (#%d) : invalid or unsupported command\n", val, count);
                break;
            }
        }
        else
        {
            fprintf(out_file, "%lf\n", val);
            next_is_command = true;
        }
        val = -1;
        count++;
    }

    
    fcloseall ();
    in_file = nullptr;
    out_file = nullptr;

    #ifdef _DEBUG
     system ("pause");
    #endif
    return 0;
}