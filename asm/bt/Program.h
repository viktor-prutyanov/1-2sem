#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>

#define END             "\xc3"
#define L_END           1

#define OUT             "\x58\x55\x48\x89\xe5\x48\x31\xd2\xbb\x0a\x00\x00\x00\x48\x31\xc9\x48\xff\xc1\x48\x31\xd2\x48\xf7\xf3\x48\x83\xc2\x30\x48\xff\xcd\x88\x55\x00\x48\x83\xf8\x00\x75\xe7\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xee\x48\x89\xca\x0f\x05\x5d"
#define L_OUT           60

#define PUSH_BYTE       "\x6a" //+ byte
#define L_PUSH_BYTE     2

#define PUSH_DWORD      "\x68" //+ dword
#define L_PUSH_DWORD    5

struct Command 
{
    uint16_t num;
    int16_t arg1;
    int16_t arg2;
    int16_t arg3;
};

class Program
{
public:
    Program(FILE *binFile);
    ~Program();

    void Dump();
    void Translate();
    void Exec();

    const uint8_t* Buf();
    size_t BufLength();

private:
    char header[8];
    uint32_t cmd_num;
    uint32_t val_num; 
    Command *cmds;
    double *vals;
    uint8_t *buffer;
    uint8_t *buf;
    long PAGESIZE;
};

Program::Program(FILE *binFile) 
{
    fread(header, 8, sizeof(char), binFile);
    fread(&val_num, 1, sizeof(uint32_t), binFile);
    fread(&cmd_num, 1, sizeof(uint32_t), binFile);

    vals = new double[val_num];
    cmds = new Command[cmd_num];

    fread(vals, val_num, sizeof(double), binFile);
    fread(cmds, cmd_num, sizeof(Command), binFile);

    PAGESIZE = sysconf(_SC_PAGESIZE);
    buffer = new uint8_t[1024 + PAGESIZE - 1];
    memset((void *)buffer, 0xc3, 1024 + PAGESIZE - 1);
    buf = (uint8_t *)(((size_t) buffer + PAGESIZE-1) & ~(PAGESIZE-1));
    mprotect(buf, 1024, PROT_READ | PROT_WRITE | PROT_EXEC);
}

void Program::Translate()
{    
    uint8_t *buf_ptr = buf;
    for (int i = 0; i < cmd_num; ++i)
    {
        switch(cmds[i].num)
        {
        case 0:     //END
            memcpy(buf_ptr, END, 1);
            buf_ptr += L_END;
            break;
        case 1:     //PUSH
            if (-128. <= vals[cmds[i].arg1] && vals[cmds[i].arg1] <= 127.)
            {
                memcpy(buf_ptr, PUSH_BYTE, 1);
                *(buf_ptr + 1) = (uint8_t)floor(vals[cmds[i].arg1]);
                buf_ptr += L_PUSH_BYTE;
            }
            else
            {
                //Maybe incorrect!
                memcpy(buf_ptr, PUSH_DWORD, 1);
                *(buf_ptr + 1) = (uint32_t)floor(vals[cmds[i].arg1]);
                buf_ptr += L_PUSH_DWORD;
            }
            break;
        case 9:     //OUT
            memcpy(buf_ptr, OUT, L_OUT);
            buf_ptr += L_OUT;
            break;
        default:
            printf("Something went wrong.\n");
            break;
        }
    }
}

void Program::Dump()
{
    printf("vals:\n");
    for (uint32_t i = 0; i < val_num; ++i)
    {
        printf("[%2d] %lf\n", i, vals[i]);
    }

    printf("cmds:\n");
    for (uint32_t i = 0; i < cmd_num; ++i)
    {
        printf("[%2d] %d (%d %d %d)\n", i, cmds[i].num, 
            cmds[i].arg1, cmds[i].arg2, cmds[i].arg3);
    }

    printf("buffer at [%p], buf at [%p]\n", buffer, buf);
    printf("pagesize = %lu\n", PAGESIZE);
    printf("buffer length = %lu\n", 1024 + PAGESIZE - 1);
    printf("buf length = %lu\n", buffer - buf + (1024 + PAGESIZE - 1));
    printf("buf:\n");
    for (int i = buf - buffer; i < 1024 + PAGESIZE - 1; ++i)
    {
        printf("\\x%.2x", buffer[i]);
    }
    printf("\n");
}

size_t Program::BufLength()
{
    return buffer - buf + (1024 + PAGESIZE - 1);
}

const uint8_t *Program::Buf()
{
    return buf;
}

void Program::Exec()
{
    ((void(*)(void))buf)();
}

Program::~Program()
{
    delete[] buffer;
    delete[] cmds;
    delete[] vals;
}