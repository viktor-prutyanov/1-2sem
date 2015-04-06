#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "Program.h"

#define CLR_RED     "\x1b[31m"
#define CLR_GRN     "\x1b[32m"
#define CLR_DFLT    "\x1b[0m"

Program *PROGRAM_PTR = nullptr; //Global pointer only for SIGSEGV handler

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr);

int main(int argc, char *argv[])
{
    bool arg_quiet = false, arg_dump = false, arg_noexec = false;
    // size_t args_len = 0;
    // switch (argc)
    // {
    // case 3:
    //     if (argv[1][0] != '-') return puts_usage();
    //     args_len = strlen(argv[1]);
    //     for (size_t i = 1; i < args_len; ++i)
    //     {
    //         switch (argv[1][i])
    //         {
    //             case 'd':
    //                 arg_dump = true;
    //                 break;
    //             case 'n':
    //                 arg_noexec = true;
    //                 break;
    //             case 'q':
    //                 arg_quiet = true;
    //                 break;
    //             default:
    //                 return puts_usage();
    //         }
    //     }
    // case 2:
    //     break;
    // default:
    //     return puts_usage();
    // }

    char opt = 0;
    char *binFileName = nullptr;

    while((opt = getopt(argc, argv, "dnqf:")) != -1)
    {
        switch(opt)
        {
        case 'd':
            arg_dump = true;
            break;
        case 'n':
            arg_noexec = true;
            break;
        case 'q':
            arg_quiet = true;
            break;
        case 'f':
            binFileName = optarg;
            break;
        default:
            puts("\
Binary translator from VPVM102 to x86_64 Linux\n\
Usage: bt [-dnvf] file\n\
 -d\tdump translation\n\
 -n\tnoexec mode (translate only)\n\
 -q\tquiet mode (errors only in output)\n\
 -f\tinput binary file");
            exit(EXIT_FAILURE);
            break;
        }
    }

    FILE *binFile = fopen(binFileName, "rb");
    if (binFile == nullptr)
    {
        printf("%s(BT_OPEN_ERROR)%s Invalid input file '%s'.\n", CLR_RED, CLR_DFLT, binFileName);
        return EXIT_FAILURE;
    }

    Program program(binFile, SigSegv_action);
    PROGRAM_PTR = &program;

    if (fclose(binFile) != 0)
    {
        printf("%s(BT_CLOSE_ERROR)%s Can't close input file '%s'.\n", 
            CLR_RED, CLR_DFLT, binFileName);
        return EXIT_FAILURE;
    }

    Command* lastCommand = program.Translate();

    FILE *dumpFile = fopen("dump.txt", "w");
    if (binFile == nullptr)
    {
        printf("%s(BT_OPEN_ERROR)%s Can't open file 'dump.txt'.\n", CLR_RED, CLR_DFLT);
        return EXIT_FAILURE;
    }

    if (lastCommand == nullptr)
    {
        if (!arg_quiet) printf("%s(BT_TRANSLATE_OK)%s Ready to exec.\n", CLR_GRN, CLR_DFLT);
        if (arg_dump) program.Dump(dumpFile);
    }
    else
    {
        printf("%s(BT_TRANSLATE_ERROR)%s Invalid command %d at %p in buf (try -d option).\n",
            CLR_RED, CLR_DFLT, lastCommand->num, program.BufPtr());
        if (arg_dump) program.Dump(dumpFile);
        if (fclose(dumpFile) != 0) printf("%s(BT_CLOSE_ERROR)%s Can't close dump file 'dump.txt'.\n", 
            CLR_RED, CLR_DFLT);
        return EXIT_FAILURE;
    }

    if (fclose(dumpFile) != 0)
    {
        printf("%s(BT_CLOSE_ERROR)%s Can't close dump file 'dump.txt'.\n", 
            CLR_RED, CLR_DFLT);
        return EXIT_FAILURE;
    }

    if (!arg_noexec) program.Exec();

    if (!arg_quiet) printf("%s(BT_EXEC_OK)%s Program exit.\n", CLR_GRN, CLR_DFLT);
    return EXIT_SUCCESS;
}

void SigSegv_action(int sig, siginfo_t *siginfo, void *ptr)
{
    printf("\n%s(BT_RUNTIME_ERROR)%s SIGSEGV (#%d) has captured at %p. Program exit.\n",
        CLR_RED, CLR_DFLT,sig, siginfo->si_addr);
    PROGRAM_PTR->~Program();
    exit(EXIT_FAILURE);
}