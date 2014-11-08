#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "..\Stack\stack.h"
#include "cpu.h"

#define STACK_SIZE 64
#define CALL_STACK_SIZE 64

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
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    char header[8];
    size_t values_size = 0;
    size_t commands_size = 0;

    fread ((void *)(header), sizeof (char), 8, in_file);
    fread ((void *)(&values_size), sizeof (size_t), 1, in_file);
    fread ((void *)(&commands_size), sizeof (size_t), 1, in_file);

    printf ("Input file is %s, size is %d bytes\n", argv[1], file_length(in_file));
    printf ("Header is {%s %d %d}\n\n", header, values_size, commands_size);

    CPU_t *cpu = (CPU_t*)calloc (1, sizeof(*cpu));

    CPU_ctor (cpu, values_size, commands_size, STACK_SIZE, CALL_STACK_SIZE);

    CPU_load (cpu, values_size, commands_size, in_file);

    CPU_dump (cpu);

    fclose (in_file);
    in_file = nullptr;

    printf ("------------------\n");

    while (!(cpu->is_end))
    {
        CPU_run_next(cpu);
        //system ("pause");
    }

    CPU_dtor (cpu);
    free (cpu);
    cpu = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif

    return 0;
}

unsigned long int file_length(FILE *file)
{
    if (file == nullptr) return 0;
    unsigned long int begin = ftell (file);
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    fseek (file, begin, 0);
    return length;
}