#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "..\Stack\stack.h"
#include "cpu.h"

#define STACK_SIZE 64

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

    size_t buffer_size = file_length (in_file) / sizeof(double);

    CPU_t *cpu = (CPU_t*)calloc (1, sizeof(*cpu));
    CPU_ctor (cpu, buffer_size, STACK_SIZE);

    CPU_load(cpu, in_file) ? printf ("Program %s with length %d loaded to CPU.\n", argv[1], buffer_size) : printf ("Error loading %s program.\n", argv[1]);

    fclose (in_file);
    in_file = nullptr;

    while (!(cpu->is_end))
    {
        CPU_run_next(cpu);
    }

    CPU_dtor(cpu);

    #ifdef _DEBUG
        system ("pause");
    #endif

    return 0;
}

unsigned long int file_length(FILE *file)
{
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    rewind (file);
    return length;
}