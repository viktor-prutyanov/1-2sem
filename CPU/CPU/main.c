#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "..\Stack\stack.h"
#include "cpu_commands.h"

#define default_stack_size 64

/**
    @brief Function that gets length of file

    @param file is pointer to file

    @return length of file in bytes 
*/
unsigned long int file_length(FILE *file);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf ("Usage:\n\tcpu <input file>\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }
    
    FILE *in_file  = fopen (argv[1], "rb");
    if (in_file == nullptr) 
    {
        printf ("Incorrect input file.\n");

        fcloseall();
        in_file = nullptr;

        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    unsigned int length = file_length (in_file) / sizeof(double);

    double *commands = (double *)calloc (length, sizeof(*commands));
    fread ((void *)commands, sizeof(double), length, in_file);
    fcloseall();

    size_t size = default_stack_size;
    Stack_t* stack = (Stack_t*) calloc(1, sizeof (*stack));
    Stack_ctor(size, stack);
    if (!Stack_ok(stack))
    {
        printf ("Stack construction failed.");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    bool success = false;
    double val = 0;

    printf("%s with length of %d successfully loaded to CPU.\n", argv[1], length);

    for (int ip = 0; ip < length; ip++)
    {
        assert (0 <=ip && ip < length);
        if (commands[ip] == 0)
        {
            printf ("Done.\n");
            break;
        }
        #define DEF_CMD(cmd, num, code, name, args)             \
        else if (num == commands[ip])                           \
        {                                                       \
            if (args == 1)                                      \
            {                                                   \
                if (ip != length - 1)                           \
                {                                               \
                    assert (0 <= ip + 1 && ip + 1 < length);    \
                    val = commands[ip + 1];                     \
                    ip++;                                       \
                }                                               \
            }                                                   \
            code;                                               \
            val = 0;                                            \
        }
        #include "..\include\commands.h"
        #undef DEF_CMD
        else
        {
            printf ("Command=%lf , ip=%d: invalid or unsupported command\n", commands[ip], ip);
            break;
        }
    }

    #ifdef _DEBUG
        system ("pause");
    #endif

    Stack_dtor(stack);

    return 0;
}

unsigned long int file_length(FILE *file)
{
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    rewind (file);
    return length;
}