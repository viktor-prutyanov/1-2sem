#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

#include "Program.h"

void handler(int sig)
{
    printf("\n\x1b[31m(BT_RUNTIME_ERROR)\x1b[0m SIGSEGV has captured! Program exit.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    bool verbose = false;
    if ((argc == 2) && (argv[1][0] == '-') && (argv[1][1] == 'v'))
    {
        verbose = true;
    }
    else if (argc == 1)
    {
        
    }
    else
    {
        printf("Binary translator from VPVM102 to x86_64 Linux\nUsage: bt [-v]\n -v\t\tverbose mode (errors only in output)\n");
        return EXIT_FAILURE;
    }

    FILE *binFile = fopen("program.bin", "rb");
    if (binFile == nullptr)
    {
        printf("\n\x1b[31m(BT_OPEN_ERROR)\x1b[0m Invalid input file.\n");
        return EXIT_FAILURE;
    }
    Program program(binFile);
    fclose(binFile);
    
    try
    {
        program.Translate(); 
    }
    catch (const std::invalid_argument& e)
    {
        printf("\n\x1b[31m(BT_TRANSLATE_ERROR)\x1b[0m %s at %p in buf.\n", 
            e.what(), program.BufPtr());
        return EXIT_FAILURE;
    }
    program.Dump();

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGSEGV); 
    act.sa_mask = set;
    sigaction(SIGSEGV, &act, 0);

    program.Exec();

    if (!verbose) printf("\x1b[32m(BT_OK)\x1b[0m Program exit.\n");
    return EXIT_SUCCESS;
}