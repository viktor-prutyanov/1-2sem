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
    printf("\n\x1b[31m(BT_ERROR)\x1b[0m SIGSEGV has captured! Program exit.\n");
    exit(EXIT_FAILURE);
}

int main()
{
    FILE *binFile = fopen("program.bin", "rb");
    if (binFile == nullptr)
    {
        printf("Invalid input file.\n");
        return EXIT_FAILURE;
    }
    Program program(binFile);
    fclose(binFile);
    
    program.Translate();
    //program.Dump();

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGSEGV); 
    act.sa_mask = set;
    sigaction(SIGSEGV, &act, 0);

    program.Exec();

    printf("\n\x1b[32m(BT_OK)\x1b[0m Program exit.\n");
    return EXIT_SUCCESS;
}