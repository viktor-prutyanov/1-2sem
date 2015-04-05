#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <vector>

#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>

#include "commands.h"

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

inline void jump_case(Jump *&jump, Command *cmds, uint8_t*& buf_ptr, size_t i,
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
    Program& operator=(const Program& other);

    void (*sigsegv_action)(int, siginfo_t *, void *);
    long page_size;

    char header[8];
    uint32_t cmd_num;
    uint32_t val_num;

    Command *cmds;
    uint8_t **cmd_ptrs;
    double *vals;
    uint8_t *buffer;
    uint8_t *buf;
    uint8_t *buf_ptr;
    
    std::vector<Jump *> jumps;
};

Program::Program(FILE *binFile, void (*sigSegv_action)(int, siginfo_t *, void *))
    :sigsegv_action (sigSegv_action), page_size (sysconf(_SC_PAGESIZE)),
    buffer (new uint8_t[1024 + page_size - 1]),
    buf ((uint8_t *)(((size_t) buffer + page_size - 1) & ~(page_size - 1))),
    buf_ptr (buf), jumps (std::vector<Jump *>())
{
    fread(header, 8, sizeof(char), binFile);
    fread(&val_num, 1, sizeof(uint32_t), binFile);
    fread(&cmd_num, 1, sizeof(uint32_t), binFile);

    vals = new double[val_num];
    cmds = new Command[cmd_num];
    cmd_ptrs = new uint8_t *[cmd_num];

    memset((void *)vals, 0, val_num * sizeof(double));
    memset((void *)cmds, 0, cmd_num * sizeof(Command));
    memset((void *)cmd_ptrs, 0, cmd_num * sizeof(uint8_t *));

    fread(vals, val_num, sizeof(double), binFile);
    fread(cmds, cmd_num, sizeof(Command), binFile);

    memset((void *)buffer, 0xc3, 1024 + page_size - 1);
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
/*MUL*/ case 7:
            memcpy(buf_ptr, MUL,  L_MUL);
            buf_ptr += L_MUL;
            break;
/*DIV*/ case 8:
            memcpy(buf_ptr, DIV,  L_DIV);
            buf_ptr += L_DIV;
            break;
/*OUT*/ case 9:     
            memcpy(buf_ptr, OUT, L_OUT);
            buf_ptr += L_OUT;
            break;
/*JMP*/ case 10:
            memcpy(buf_ptr, JMP, 1);
            jump_case(jump, cmds, buf_ptr, i, jumps, false, L_JMP);
            break;
/*JE*/  case 11:
            memcpy(buf_ptr, JE,  7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JNE*/ case 12:
            memcpy(buf_ptr, JNE, 7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JA*/  case 13:
            memcpy(buf_ptr, JA,  7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JAE*/ case 14:
            memcpy(buf_ptr, JAE, 7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JB*/  case 15:
            memcpy(buf_ptr, JB,  7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JBE*/ case 16:
            memcpy(buf_ptr, JBE, 7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JZ*/  case 17:
            memcpy(buf_ptr, JZ,  7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JNZ*/ case 18:
            memcpy(buf_ptr, JNZ, 7);
            jump_case(jump, cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*MOV*/ case 19:
            if ((-5 < cmds[i].arg1 && cmds[i].arg1 < 0) && (-5 < cmds[i].arg2 && cmds[i].arg2 < 0))
            {
                memcpy(buf_ptr, MOV_REG_REG, 2);
                *(buf_ptr + 2) = (uint8_t)('\xda' - cmds[i].arg1 - '\x08' * (cmds[i].arg2 + 1));
                buf_ptr += L_MOV_REG_REG;
            }
            else if ((-5 < cmds[i].arg1 && cmds[i].arg1 < 0) && (cmds[i].arg2 >= 0))
            {
                memcpy(buf_ptr, MOV_REG_NUM, 1);
                *(buf_ptr + 1) = (uint8_t)('\xba' - cmds[i].arg1);
                *((uint32_t *)(buf_ptr + 2))= (uint32_t)floor(vals[cmds[i].arg2]);
                buf_ptr += L_MOV_REG_NUM;
            }
            else return cmds + i;
            break;
/*SWAP*/case 20:
            memcpy(buf_ptr, SWAP, L_SWAP);
            buf_ptr += L_SWAP;
            break;
/*CALL*/case 21:
            memcpy(buf_ptr, CALL, 1);
            jump_case(jump, cmds, buf_ptr, i, jumps, false, L_CALL);
            break;
/*RET*/ case 22:
            memcpy(buf_ptr, RET, L_RET);
            buf_ptr += L_RET;
            break;
/*DUP*/ case 23:    
            memcpy(buf_ptr, DUP, L_DUP);
            buf_ptr += L_DUP;
            break;
/*IN*/  case 24:
            memcpy(buf_ptr, IN, L_IN);
            buf_ptr += L_IN;
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

inline void jump_case(Jump *&jump, Command *cmds, uint8_t*& buf_ptr, size_t i, 
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