#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
//#include <limits.h>

#define PAGESIZE 4096

void handler(int sig)
{
    printf("SIGSEGV!\n");
    exit(EXIT_FAILURE);
}

int main()
{
    char program[] = { 0xbf, 0x0f, 0x00, 0x00, 0x00, 0xb8, 0x3c, 
        0x00, 0x00, 0x00, 0x0f, 0x05 }; //Return with exitcode 15.

    char *buffer;
    buffer = (char *)malloc(1024 + PAGESIZE - 1);
    if (buffer == NULL) 
    {
        printf("Malloc failed.\n");
        return EXIT_FAILURE;
    }

    memset(buffer, 0xc3, 1024 + PAGESIZE - 1);
    char *buf = (char *)(((size_t) buffer + PAGESIZE-1) & ~(PAGESIZE-1));

    if (mprotect(buf, 1024, PROT_READ | PROT_WRITE | PROT_EXEC)) 
    {
        printf("Mprotect failed.\n");
        return EXIT_FAILURE;
    }

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGSEGV); 
    act.sa_mask = set;
    sigaction(SIGSEGV, &act, 0);

    memcpy(buf, program, sizeof(program)/sizeof(char));

    ((void(*)(void))buf)();  //Cause error 15;

    printf("Success.");

    return EXIT_SUCCESS;
}