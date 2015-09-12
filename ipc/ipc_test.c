
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#define CLR_RED     "\x1b[31m"
#define CLR_GRN     "\x1b[32m"
#define CLR_DFLT    "\x1b[0m"

int main() 
{
    int status, died;
    int pid = fork();
    switch(pid) 
    {
    case -1: 
        printf("Fork failed.\n");
        exit(-1);
    case 0: 
        printf("Child of %d with %d\n", getppid(), getpid());
        for (int i = 0; i < 3; ++i)
        {
            sleep(1);
            printf("\x1b[35mChild: %d\x1b[0m\n", 3 - i);
        }
        exit(0);
    default: 
        printf("Parent with %d\n", getpid());
        for (int i = 0; i < 12; ++i)
        {
            sleep(1);
            printf("\x1b[31mParent: %d\x1b[0m\n", 12 - i);
        }
        if (pid & 1) kill(pid, SIGKILL);
        died = wait(&status);
    }
}