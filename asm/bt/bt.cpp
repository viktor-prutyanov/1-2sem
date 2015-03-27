#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
//#include <limits.h>

void handler(int sig)
{
    printf("SIGSEGV!\n");
    exit(EXIT_FAILURE);
}

int main()
{
    long PAGESIZE = sysconf(_SC_PAGESIZE);

    //uint8_t program[] = "\xb8\x3c\x00\x00\x00\xbf\x03\x00\x00\x00\x0f\x05"; //Exit only
    //uint8_t program[] = "\x48\x89\xe5\x6a\x56\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xee\x48\x83\xee\x08\xba\x01\x00\x00\x00\x0f\x05\x58\xb8\x3c\x00\x00\x00\xbf\x00\x00\x00\x00\x0f\x05"; //Prints V
    uint8_t print_program[] = "\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xe6\xba\x01\x00\x00\x00\x0f\x05";

    uint8_t *buffer;

    buffer = (uint8_t *)malloc(1024 + PAGESIZE - 1);
    if (buffer == NULL) 
    {
        printf("Malloc failed.\n");
        return EXIT_FAILURE;
    }

    memset(buffer, 0xc3, 1024 + PAGESIZE - 1);
    uint8_t *buf = (uint8_t *)(((size_t) buffer + PAGESIZE-1) & ~(PAGESIZE-1));

    if (mprotect(buf, 1024, PROT_READ | PROT_WRITE | PROT_EXEC)) 
    {
        printf("Mprotect failed.\n");
        return EXIT_FAILURE;
    }

    printf("BT: Program started.\n");

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGSEGV); 
    act.sa_mask = set;
    sigaction(SIGSEGV, &act, 0);

    memcpy(buf, program, sizeof(program)/sizeof(uint8_t));

    ((void(*)(void))buf)();

    printf("\nBT: Program finished.\n");

    return EXIT_SUCCESS;
}