#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <vector>

#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>

#define END                 "\xc3"
#define L_END               1

#define OUT                 "\x58\x55\x48\x89\xe5\x48\xff\xcd\xc6\x45\x00\x0a\x48\x31\xd2\xbb\x0a\x00\x00\x00\xb9\x01\x00\x00\x00\x48\xff\xc1\x48\x31\xd2\x48\xf7\xf3\x48\x83\xc2\x30\x48\xff\xcd\x88\x55\x00\x48\x83\xf8\x00\x75\xe7\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x89\xee\x48\x89\xca\x0f\x05\x5d"
#define L_OUT               69

#define PUSH_BYTE           "\x6a" //+ byte
#define L_PUSH_BYTE         2

#define PUSH_DWORD          "\x68" //+ dword
#define L_PUSH_DWORD        5

#define PUSH_REG            "\x41" //+ from '\x53' to '\x56' : from r11 to r14
#define L_PUSH_REG          2

#define POP_REG             "\x41" //+ from '\x5b' to '\x5e' : from r11 to r14
#define L_POP_REG           2

#define ADD                 "\x58\x48\x01\x04\x24"
#define L_ADD               5
    
#define SUB                 "\x5b\x58\x48\x29\xc3\x53"
#define L_SUB               6
    
#define NOP                 "\x90"
#define L_NOP               1

#define DUP                 "\x58\x50\x50"
#define L_DUP               3

#define SWAP                "\x58\x5b\x50\x53"
#define L_SWAP              4

#define JMP                 "\xe9" //+ dword (relative address) 
#define L_JMP               5

#define JB                  "\x58\x5b\x48\x39\xd8\x0f\x82" //+ dword (relative address)
#define JAE                 "\x58\x5b\x48\x39\xd8\x0f\x83" //
#define JE                  "\x58\x5b\x48\x39\xd8\x0f\x84" // 
#define JNE                 "\x58\x5b\x48\x39\xd8\x0f\x85" //
#define JBE                 "\x58\x5b\x48\x39\xd8\x0f\x86" //
#define JA                  "\x58\x5b\x48\x39\xd8\x0f\x87" //

#define L_JXX               11 


struct Command 
{
    uint16_t num;
    int16_t arg1;
    int16_t arg2;
    int16_t arg3;
};

struct Jump
{
    Command *cmd_from;
    uint8_t *ptr_from;
    Command *cmd_to;
    uint8_t *ptr_to;
    bool conditional;
};

inline void jxx_case(Jump *&jump, Command *cmds, uint8_t*& buf_ptr, size_t i,
    std::vector<Jump *>& jumps, bool conditional, size_t length);

class Program
{
public:
    Program(FILE *binFile, void (*sigSegv_action)(int, siginfo_t *, void *));
    ~Program();

    void Dump(FILE *dumpFile);
    Command* Translate();
    void Exec();

    const uint8_t *Buf();
    const uint8_t *BufPtr();
    size_t BufLength();

private:
    char header[8];
    uint32_t cmd_num;
    uint32_t val_num; 
    Command *cmds;
    uint8_t **cmd_ptrs;
    double *vals;
    uint8_t *buffer;
    uint8_t *buf;
    uint8_t *buf_ptr;
    long page_size;
    std::vector<Jump *> jumps;
    void (*sigsegv_action)(int, siginfo_t *, void *);
};

Program::Program(FILE *binFile, void (*sigSegv_action)(int, siginfo_t *, void *)) 
{
    sigsegv_action = sigSegv_action;

    fread(header, 8, sizeof(char), binFile);
    fread(&val_num, 1, sizeof(uint32_t), binFile);
    fread(&cmd_num, 1, sizeof(uint32_t), binFile);

    vals = new double[val_num];
    cmds = new Command[cmd_num];
    cmd_ptrs = new uint8_t *[cmd_num];

    fread(vals, val_num, sizeof(double), binFile);
    fread(cmds, cmd_num, sizeof(Command), binFile);

    page_size = sysconf(_SC_PAGESIZE);
    buffer = new uint8_t[1024 + page_size - 1];
    memset((void *)buffer, 0xc3, 1024 + page_size - 1);
    buf = (uint8_t *)(((size_t) buffer + page_size - 1) & ~(page_size - 1));
    buf_ptr = buf;
    mprotect(buf, 1024, PROT_READ | PROT_WRITE | PROT_EXEC);
}

Command* Program::Translate()
{
    buf_ptr = buf;
    Jump *jump;
    for (size_t i = 0; i < cmd_num; ++i)
    {
        cmd_ptrs[i] = buf_ptr;
        switch(cmds[i].num)
        {
/*END*/ case 0:    
            memcpy(buf_ptr, END, 1);
            buf_ptr += L_END;
            break;
/*PUSH*/case 1:
            if (-5 < cmds[i].arg1 && cmds[i].arg1 < 0)
            {
                memcpy(buf_ptr, PUSH_REG, 1);
                *(buf_ptr + 1) = (uint8_t)('\x52' - cmds[i].arg1);
                buf_ptr += L_PUSH_REG;
            }
            else
            {
                if (-128. <= vals[cmds[i].arg1] && vals[cmds[i].arg1] <= 127.)
                {
                    memcpy(buf_ptr, PUSH_BYTE, 1);
                    *(buf_ptr + 1) = (uint8_t)floor(vals[cmds[i].arg1]);
                    buf_ptr += L_PUSH_BYTE;
                }
                else
                {
                    memcpy(buf_ptr, PUSH_DWORD, 1);
                    *((uint32_t *)(buf_ptr + 1))= (uint32_t)floor(vals[cmds[i].arg1]);
                    buf_ptr += L_PUSH_DWORD;
                }
            }
            break;
/*POP*/ case 2:
            if (-5 < cmds[i].arg1 && cmds[i].arg1 < 0)
            {
                memcpy(buf_ptr, POP_REG, 1);
                *(buf_ptr + 1) = (uint8_t)('\x5a' - cmds[i].arg1);
                buf_ptr += L_POP_REG;
            }
            break;
/*NOP*/ case 4:
            memcpy(buf_ptr, NOP,  L_NOP);
            buf_ptr += L_NOP;
            break;
/*ADD*/ case 5:    
            memcpy(buf_ptr, ADD,  L_ADD);
            buf_ptr += L_ADD;
            break;
/*SUB*/ case 6:    
            memcpy(buf_ptr, SUB,  L_SUB);
            buf_ptr += L_SUB;
            break;  
/*OUT*/ case 9:     
            memcpy(buf_ptr, OUT, L_OUT);
            buf_ptr += L_OUT;
            break;
/*JMP*/case 10:
            memcpy(buf_ptr, JMP, 1);
            jxx_case(jump, cmds, buf_ptr, i, jumps, false, L_JMP);
            break;
/*JE*/  case 11:
            memcpy(buf_ptr, JE, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JNE*/ case 12:
            memcpy(buf_ptr, JNE, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JA*/  case 13:
            memcpy(buf_ptr, JA, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JAE*/ case 14:
            memcpy(buf_ptr, JAE, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JB*/  case 15:
            memcpy(buf_ptr, JB, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JBE*/ case 16:
            memcpy(buf_ptr, JBE, 7);
            jxx_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*SWAP*/case 20:
            memcpy(buf_ptr, SWAP, L_SWAP);
            buf_ptr += L_SWAP;
            break;
/*DUP*/ case 23:    
            memcpy(buf_ptr, DUP, L_DUP);
            buf_ptr += L_DUP;
            break;
        default: 
            return cmds + i;
            break;
        }
    }

    for (long int i = 0; i < cmd_num; ++i)
    {
        for (size_t j = 0; j < jumps.size(); ++j)
        {
            if (i == (jumps[j]->cmd_to) - cmds)
            {
                jumps[j]->ptr_to = cmd_ptrs[i];
                if (jumps[j]->conditional)
                {
                    *((uint32_t *)(jumps[j]->ptr_from + 7)) = (uint32_t)(cmd_ptrs[i] - jumps[j]->ptr_from - 11);
                }
                else
                {
                    *((uint32_t *)(jumps[j]->ptr_from + 1)) = (uint32_t)(cmd_ptrs[i] - jumps[j]->ptr_from - 5);
                }
            }
        }
    }

    return nullptr;
}

void Program::Dump(FILE *dumpFile)
{
    fprintf(dumpFile, "vals:\n");
    for (size_t i = 0; i < val_num; ++i)
    {
        fprintf(dumpFile, "[%2lu] %lf\n", i, vals[i]);
    }

    fprintf(dumpFile, "\ncmds:\n");
    for (size_t i = 0; i < cmd_num; ++i)
    {
        fprintf(dumpFile, "[%2lu] %2d (%6d %6d %6d) at %p\n", i, cmds[i].num, 
            cmds[i].arg1, cmds[i].arg2, cmds[i].arg3, cmd_ptrs[i]);
    }

    fprintf(dumpFile, "\nbuffer at [%p], buf at [%p]\n", buffer, buf);
    fprintf(dumpFile, "page_size = %lu\n", page_size);
    fprintf(dumpFile, "buffer length = %lu\n", 1024 + page_size - 1);
    fprintf(dumpFile, "buf length = %lu\n", buffer - buf + (1024 + page_size - 1));
    fprintf(dumpFile, "\nbuf:\n");
    for (long int i = buf - buffer; i < 1024 + page_size - 1; ++i)
    {
        fprintf(dumpFile, " %.2x", buffer[i]);
    }
    fprintf(dumpFile, "\n\njumps:\n");
    for (size_t i = 0; i < jumps.size(); ++i)
    {
        fprintf(dumpFile, "From %lu at %p, to %lu at %p\n", 
            (jumps[i]->cmd_from - cmds), jumps[i]->ptr_from,
            (jumps[i]->cmd_to - cmds), jumps[i]->ptr_to);
    }
}

size_t Program::BufLength()
{
    return buffer - buf + (1024 + page_size - 1);
}

const uint8_t *Program::Buf()
{
    return buf;
}

const uint8_t *Program::BufPtr()
{
    return buf_ptr;
}


void Program::Exec()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = Program::sigsegv_action;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGSEGV); 
    act.sa_mask = set;
    sigaction(SIGSEGV, &act, 0);

    ((void(*)(void))buf)();
}

Program::~Program()
{
    for (size_t i = 0; i < jumps.size(); ++i)
    {
        free(jumps[i]);
    }
    delete[] cmd_ptrs;
    delete[] buffer;
    delete[] cmds;
    delete[] vals;
}

inline void jxx_case(Jump *&jump, Command *cmds, uint8_t*& buf_ptr, size_t i, 
    std::vector<Jump *>& jumps, bool conditional, size_t length)
{
    jump = (Jump *)malloc(sizeof(Jump));
    jump->ptr_from = buf_ptr;
    jump->ptr_to = nullptr;
    jump->cmd_from = &(cmds[i]);
    jump->cmd_to = &(cmds[cmds[i].arg1]);
    jump->conditional = conditional;
    jumps.push_back(jump);
    buf_ptr += length;
}