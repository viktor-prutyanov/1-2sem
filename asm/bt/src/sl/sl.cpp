/**
*   Simple linker for binary translation
*
*   @file sl.cpp
*
*   @date 03.2015 - 04.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>

#include "Elf.h"

#define CLR_RED     "\x1b[31m"
#define CLR_GRN     "\x1b[32m"
#define CLR_DFLT    "\x1b[0m"

int main(int argc, char *argv[])
{
    bool arg_dump = false;
    char opt = 0;
    char *elfFileName = nullptr, *objFileName = nullptr;

    while((opt = getopt(argc, argv, "di:o:")) != -1)
    {
        switch(opt)
        {
        case 'd':
            arg_dump = true;
            break;
        case 'i':
            objFileName = optarg;
            break;
        case 'o':
            elfFileName = optarg;
            break;
        default:
            puts("\
Simple linker from bt object files to x86_64 Linux ELF\n\
Usage: sl [-d] [-i file] [-o file]\n\
 -d\tdump link to sl_dump.txt\n\
 -o\toutput executable file (for sl)\n\
 -i\tinput object file");
            exit(EXIT_FAILURE);
            break;
        }
    }

    FILE *objFile = fopen(objFileName, "rb");
    if (objFile == nullptr)
    {
        printf("%s(SL_OPEN_ERROR)%s Invalid input file '%s'.\n", CLR_RED, CLR_DFLT, objFileName);
        return EXIT_FAILURE;
    }

    Elf elf(objFile);

    if (fclose(objFile) != 0)
    {
        printf("%s(SL_CLOSE_ERROR)%s Can't close input file '%s'.\n", 
            CLR_RED, CLR_DFLT, objFileName);
        return EXIT_FAILURE;
    }

    
    elf.Link();
    
    if (arg_dump)
    {
        FILE *dumpFile = fopen("sl_dump.txt", "w");
        if (dumpFile == nullptr)
        {
            printf("%s(SL_OPEN_ERROR)%s Invalid input file 'sl_dump.txt'.\n", CLR_RED, CLR_DFLT);
            return EXIT_FAILURE;
        }
        
        elf.Dump(dumpFile);
    
        if (fclose(dumpFile) != 0)
        {
                printf("%s(SL_CLOSE_ERROR)%s Can't close input file 'sl_dump.txt'.\n", 
                    CLR_RED, CLR_DFLT);
                return EXIT_FAILURE;
        }
    }

    FILE *elfFile = fopen(elfFileName, "wb");
    if (elfFile == nullptr)
    {
        printf("%s(SL_OPEN_ERROR)%s Invalid elf file '%s'.\n", CLR_RED, CLR_DFLT, elfFileName);
        return EXIT_FAILURE;
    }

    elf.Store(elfFile);

    if (fclose(elfFile) != 0)
    {
        printf("%s(SL_CLOSE_ERROR)%s Can't close elf file '%s'.\n", 
            CLR_RED, CLR_DFLT, elfFileName);
        return EXIT_FAILURE;
    }

    if (chmod(elfFileName, 0777) != 0)
    {
        printf("%s(SL_CHMOD_ERROR)%s Can't set executable permissions for elf file '%s'.\n", 
            CLR_RED, CLR_DFLT, elfFileName);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}