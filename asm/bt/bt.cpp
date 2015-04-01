#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

#include "Program.h"

inline int puts_usage()
{
    puts("\
Binary translator from VPVM102 to x86_64 Linux\n\
Usage: bt [-dnv] [binfile]\n\
 -d\tdump translation\n\
 -n\tnoexec mode (translate only)\n\
 -v\tverbose mode (errors only in output)");
    return EXIT_FAILURE;
}

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr);

int main(int argc, char *argv[])
{
    bool arg_verbose, arg_dump, arg_noexec;

    switch (argc)
    {
    case 3:
        if (argv[1][0] != '-') return puts_usage();
        for (size_t i = 1; i < strlen(argv[1]); ++i)
        {
            switch (argv[1][i])
            {
                case 'd':
                    arg_dump = true;
                    break;
                case 'n':
                    arg_noexec = true;
                    break;
                case 'v':
                    arg_verbose = true;
                    break;
                default:
                    return puts_usage();
            }
        }
    case 2:
        break;
    default:
        return puts_usage();
    }

    FILE *binFile = fopen(argv[argc - 1], "rb");
    if (binFile == nullptr)
    {
        printf("\x1b[31m(BT_LOAD_ERROR)\x1b[0m Invalid input file '%s'.\n", argv[argc - 1]);
        return EXIT_FAILURE;
    }
    Program program(binFile, SigSegv_action);
    fclose(binFile);

    Command* lastCommand = program.Translate();

    FILE *dumpFile = fopen("dump.txt", "w");
    if (lastCommand == nullptr)
    {
        if (!arg_verbose) printf("\x1b[32m(BT_TRANSLATE_OK)\x1b[0m Ready to exec.\n");
        if (arg_dump) program.Dump(dumpFile);
    }
    else
    {
        printf("\x1b[31m(BT_TRANSLATE_ERROR)\x1b[0m Invalid command %d at %p in buf (try -d option).\n", 
            lastCommand->num, program.BufPtr());
        if (arg_dump) program.Dump(dumpFile);
        return EXIT_FAILURE;
    }
    fclose(dumpFile);

    if (!arg_noexec) program.Exec();

    if (!arg_verbose) printf("\x1b[32m(BT_EXEC_OK)\x1b[0m Program exit.\n");
    return EXIT_SUCCESS;
}

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr)
{
    printf("\n\x1b[31m(BT_RUNTIME_ERROR)\x1b[0m SIGSEGV (#%d) has captured at %p. Program exit.\n", sig, siginfo->si_addr);

    exit(EXIT_FAILURE);
}