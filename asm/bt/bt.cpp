#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

#include "Program.h"

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr);

int main(int argc, char *argv[])
{
    bool verbose, dump, noexec;

    if (argc == 1)
    {
        
    }
    else 
    {
        if ((argc == 2) && (argv[1][0] == '-'))
        {
            dump    = (strchr(argv[1], 'd') != nullptr);
            verbose = (strchr(argv[1], 'v') != nullptr);
            noexec  = (strchr(argv[1], 'n') != nullptr); 
        }
        
        if (!(dump || verbose || noexec))
        {
            puts("\
Binary translator from VPVM102 to x86_64 Linux\n\
Usage: bt [-dnv]\n\
 -d\t\tdump translation\n\
 -n\t\tnoexec mode (translate only)\n\
 -v\t\tverbose mode (errors only in output)");
        return EXIT_FAILURE;
        }
    }

    FILE *binFile = fopen("program.bin", "rb");
    if (binFile == nullptr)
    {
        printf("\x1b[31m(BT_LOAD_ERROR)\x1b[0m Invalid input file.\n");
        return EXIT_FAILURE;
    }
    Program program(binFile, SigSegv_action);
    fclose(binFile);

    Command* lastCommand = program.Translate();

    FILE *dumpFile = fopen("program.dump", "w");
    if (lastCommand == nullptr)
    {
        if (!verbose) printf("\x1b[32m(BT_TRANSLATE_OK)\x1b[0m Ready to exec.\n");
        if (dump) program.Dump(dumpFile);
    }
    else
    {
        printf("\x1b[31m(BT_TRANSLATE_ERROR)\x1b[0m Invalid command %d at %p in buf (try -d option).\n", 
            lastCommand->num, program.BufPtr());
        if (dump) program.Dump(dumpFile);
        return EXIT_FAILURE;
    }
    fclose(dumpFile);

    if (!noexec) program.Exec();

    if (!verbose) printf("\x1b[32m(BT_EXEC_OK)\x1b[0m Program exit.\n");
    return EXIT_SUCCESS;
}

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr)
{
    printf("\n\x1b[31m(BT_RUNTIME_ERROR)\x1b[0m SIGSEGV (#%d) has captured at %p. Program exit.\n", sig, siginfo->si_addr);

    exit(EXIT_FAILURE);
}