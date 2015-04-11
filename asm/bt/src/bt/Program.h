/**
*   Program class for bt
*
*   @file Program.h
*
*   @date 03.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <vector>
#include <stdexcept>

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
    Command *cmd_to;
    uint8_t *ptr_from;
    uint8_t *ptr_to;
    bool conditional;
    bool visible;
};

inline void jump_case(Command *cmds, uint8_t*& buf_ptr, size_t i,
    std::vector<Jump *>& jumps, bool conditional, size_t length);

inline void add_invisible_jump(uint8_t* ptr_to, std::vector<Jump *>& jumps);

class Program
{
public:
    Program(FILE *binFile, void (*sigSegv_action)(int, siginfo_t *, void *));
    ~Program();

    void Dump(FILE *dumpFile);
    Command* Translate();
    void Exec();
    void GenerateObj(FILE *objFile);

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

    uint8_t *buffer;
    uint8_t *buf;
    uint8_t *origin_buf;
    uint8_t *buf_ptr;
    double *vals;
    Command *cmds;
    uint8_t **cmd_ptrs;
    size_t end_offset;

    std::vector<Jump *> jumps;
};

Program::Program(FILE *binFile, void (*sigSegv_action)(int, siginfo_t *, void *))
   :sigsegv_action (sigSegv_action), 
    page_size (sysconf(_SC_PAGESIZE)),
    buffer (new uint8_t[1024 + page_size - 1]),
    buf ((uint8_t *)(((size_t) buffer + page_size - 1) & ~(page_size - 1))),
    origin_buf (buf),
    buf_ptr (buf),
    vals (nullptr), 
    cmds (nullptr), 
    cmd_ptrs (nullptr),
    end_offset (0),
    jumps (std::vector<Jump *>())
{
    if (binFile == nullptr) throw std::invalid_argument("Null pointer as input file.");

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

    memset((void *)buffer, '\xc3', 1024 + page_size - 1);
    mprotect(buf, 1024, PROT_READ | PROT_WRITE | PROT_EXEC);
}

Command* Program::Translate()
{
    memcpy(buf, OUT, INSTR_SIZEOF(OUT));
    buf += INSTR_SIZEOF(OUT);
    memcpy(buf, RET, INSTR_SIZEOF(RET));
    buf += INSTR_SIZEOF(RET);
    memcpy(buf, IN, INSTR_SIZEOF(IN));
    buf += INSTR_SIZEOF(IN);
    memcpy(buf, RET, INSTR_SIZEOF(RET));
    buf += INSTR_SIZEOF(RET);

    buf_ptr = buf;
    for (size_t i = 0; i < cmd_num; ++i)
    {
        cmd_ptrs[i] = buf_ptr;
        switch(cmds[i].num)
        {
/*END*/ case 0:    
            memcpy(buf_ptr, END, INSTR_SIZEOF(END));
            end_offset = buf_ptr - origin_buf;
            buf_ptr += L_END;
            break;
/*PUSH*/case 1:
            if (-5 < cmds[i].arg1 && cmds[i].arg1 < 0)
            {
                memcpy(buf_ptr, PUSH_REG, INSTR_SIZEOF(PUSH_REG));
                *(buf_ptr + INSTR_SIZEOF(PUSH_REG)) = (uint8_t)(PUSH_R_RBASE - cmds[i].arg1);
                buf_ptr += L_PUSH_REG;
            }
            else
            {
                if (-128. <= vals[cmds[i].arg1] && vals[cmds[i].arg1] <= 127.)
                {
                    memcpy(buf_ptr, PUSH_BYTE, sizeof(PUSH_BYTE) - 1);
                    *(buf_ptr + INSTR_SIZEOF(PUSH_BYTE)) = (uint8_t)floor(vals[cmds[i].arg1]);
                    buf_ptr += L_PUSH_BYTE;
                }
                else
                {
                    memcpy(buf_ptr, PUSH_DWORD, INSTR_SIZEOF(PUSH_DWORD));
                    *((uint32_t *)(buf_ptr + INSTR_SIZEOF(PUSH_DWORD))) = (uint32_t)floor(vals[cmds[i].arg1]);
                    buf_ptr += L_PUSH_DWORD;
                }
            }
            break;
/*POP*/ case 2:
            if (-5 < cmds[i].arg1 && cmds[i].arg1 < 0)
            {
                memcpy(buf_ptr, POP_REG, INSTR_SIZEOF(POP_REG));
                *(buf_ptr + INSTR_SIZEOF(POP_REG)) = (uint8_t)(POP_R_RBASE - cmds[i].arg1);
                buf_ptr += L_POP_REG;
            }
            break;
/*NOP*/ case 4:
            memcpy(buf_ptr, NOP,  L_NOP);
            buf_ptr += L_NOP;
            break;
/*ADD*/ case 5:     // See ../../doc/ADD.asm
            memcpy(buf_ptr, ADD,  L_ADD);
            buf_ptr += L_ADD;
            break;
/*SUB*/ case 6:     // See ../../doc/SUB.asm
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
/*OUT*/ case 9:     // See ../../doc/OUT.asm
            memcpy(buf_ptr, CALL, INSTR_SIZEOF(CALL));
            *((uint32_t *)(buf_ptr + INSTR_SIZEOF(CALL))) = (uint32_t)(origin_buf - buf_ptr - L_CALL);
            buf_ptr += L_CALL;
            add_invisible_jump(origin_buf, jumps);
            memcpy(buf_ptr, BALANCE_POP_RAX, INSTR_SIZEOF(BALANCE_POP_RAX));        //Just for stack balance
            buf_ptr += INSTR_SIZEOF(BALANCE_POP_RAX);
            break;
/*JMP*/ case 10:
            memcpy(buf_ptr, JMP, INSTR_SIZEOF(JMP));
            jump_case(cmds, buf_ptr, i, jumps, false, L_JMP);
            break;
/*JE*/  case 11:
            memcpy(buf_ptr, JE,  INSTR_SIZEOF(JE) );
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JNE*/ case 12:
            memcpy(buf_ptr, JNE, INSTR_SIZEOF(JNE));
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JA*/  case 13:
            memcpy(buf_ptr, JA,  INSTR_SIZEOF(JA) );
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JAE*/ case 14:
            memcpy(buf_ptr, JAE, INSTR_SIZEOF(JAE));
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JB*/  case 15:
            memcpy(buf_ptr, JB,  INSTR_SIZEOF(JB) );
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JBE*/ case 16:
            memcpy(buf_ptr, JBE, INSTR_SIZEOF(JBE));
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JZ*/  case 17:
            memcpy(buf_ptr, JZ,  INSTR_SIZEOF(JZ) );
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*JNZ*/ case 18:
            memcpy(buf_ptr, JNZ, INSTR_SIZEOF(JNZ));
            jump_case(cmds, buf_ptr, i, jumps, true, L_JXX);
            break;
/*MOV*/ case 19:
            if ((-5 < cmds[i].arg1 && cmds[i].arg1 < 0) && (-5 < cmds[i].arg2 && cmds[i].arg2 < 0))
            {
                memcpy(buf_ptr, MOV_REG_REG, INSTR_SIZEOF(MOV_REG_REG));
                *(buf_ptr + INSTR_SIZEOF(MOV_REG_REG)) = 
                    (uint8_t)(MOV_RR_RBASE - cmds[i].arg1 - MOV_RR_RSTEP * (cmds[i].arg2 + 1));
                buf_ptr += L_MOV_REG_REG;
            }
            else if ((-5 < cmds[i].arg1 && cmds[i].arg1 < 0) && (cmds[i].arg2 >= 0))
            {
                memcpy(buf_ptr, MOV_REG_NUM, INSTR_SIZEOF(MOV_REG_NUM));
                *(buf_ptr + INSTR_SIZEOF(MOV_REG_NUM)) = (uint8_t)(MOV_RN_RBASE - cmds[i].arg1);
                *((uint32_t *)(buf_ptr + INSTR_SIZEOF(MOV_REG_NUM) + 1))= (uint32_t)floor(vals[cmds[i].arg2]);
                buf_ptr += L_MOV_REG_NUM;
            }
            else return cmds + i;
            break;
/*SWAP*/case 20:
            memcpy(buf_ptr, SWAP, L_SWAP);
            buf_ptr += L_SWAP;
            break;
/*CALL*/case 21:
            memcpy(buf_ptr, CALL, INSTR_SIZEOF(CALL));
            jump_case(cmds, buf_ptr, i, jumps, false, L_CALL);
            break;
/*RET*/ case 22:
            memcpy(buf_ptr, RET, L_RET);
            buf_ptr += L_RET;
            break;
/*DUP*/ case 23:    // See ../../doc/DUP.asm  
            memcpy(buf_ptr, DUP, L_DUP);
            buf_ptr += L_DUP;
            break;
/*IN*/  case 24:    // See ../../doc/IN.asm
            memcpy(buf_ptr, CALL, INSTR_SIZEOF(CALL));
            *((uint32_t *)(buf_ptr + INSTR_SIZEOF(CALL))) = 
                (uint32_t)(origin_buf + INSTR_SIZEOF(OUT) + INSTR_SIZEOF(RET) - buf_ptr - L_CALL);
            add_invisible_jump(origin_buf + INSTR_SIZEOF(OUT) + INSTR_SIZEOF(RET), jumps);
            buf_ptr += L_CALL;
            memcpy(buf_ptr, BALANCE_PUSH_RAX, INSTR_SIZEOF(BALANCE_PUSH_RAX));      //Just for stack balance
            buf_ptr += INSTR_SIZEOF(BALANCE_PUSH_RAX);
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
            if ((i == (jumps[j]->cmd_to) - cmds) && (jumps[j]->visible))
            {
                jumps[j]->ptr_to = cmd_ptrs[i];
                if (jumps[j]->conditional)
                {
                    *((uint32_t *)(jumps[j]->ptr_from + 7)) = 
                        (uint32_t)(cmd_ptrs[i] - jumps[j]->ptr_from - 11);
                }
                else
                {
                    *((uint32_t *)(jumps[j]->ptr_from + 1)) = 
                        (uint32_t)(cmd_ptrs[i] - jumps[j]->ptr_from - 5);
                }
            }
        }
    }

    return nullptr;
}

void Program::GenerateObj(FILE *objFile)
{
    if (objFile == nullptr) throw std::invalid_argument("Null pointer as object file.");

    size_t origin_buf_len = buf_ptr - origin_buf;
    size_t jumps_size = jumps.size();
    fwrite(&origin_buf_len, 1, sizeof(size_t), objFile);
    fwrite(&jumps_size,     1, sizeof(size_t), objFile);
    /*TODO: Add jumps from IN and OUT routines*/
    fwrite(origin_buf, origin_buf_len, sizeof(uint8_t), objFile);

    for (size_t i = 0; i < jumps_size; ++i)
    {
        fwrite(&(jumps[i]->ptr_to), 1, sizeof(uint8_t *), objFile);
    }

    fwrite(&origin_buf, 1, sizeof(uint8_t *), objFile);

    size_t start_offset = buf - origin_buf;
    fwrite(&start_offset, 1, sizeof(size_t), objFile);
    fwrite(&end_offset,   1, sizeof(size_t), objFile);
}

void Program::Dump(FILE *dumpFile)
{
    if (dumpFile == nullptr) throw std::invalid_argument("Null pointer as dump file.");

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

    fprintf(dumpFile, "\nbuffer at [%p], buf at [%p], origin_buf at [%p]\n", buffer, buf, origin_buf);
    fprintf(dumpFile, "page_size = %lu\n", page_size);
    fprintf(dumpFile, "buffer length = %lu\n", 1024 + page_size - 1);
    fprintf(dumpFile, "buf length = %lu\n", buffer - buf + (1024 + page_size - 1));

    fprintf(dumpFile, "\norigin_buf:\n");
    for (uint8_t *p = origin_buf; p < buf; ++p)
    {
        fprintf(dumpFile, " %.2x", *p);
    }

    fprintf(dumpFile, "\nbuf:\n");
    for (uint8_t *p = buf; p < buffer + 1024 + page_size - 1; ++p)
    {
        fprintf(dumpFile, " %.2x", *p);
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
    struct sigaction act = {};
    act.sa_sigaction = Program::sigsegv_action;
    sigset_t set = {}; 
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
        delete jumps[i];
    }
    delete[] cmd_ptrs;
    delete[] buffer;
    delete[] cmds;
    delete[] vals;
}

inline void jump_case(Command *cmds, uint8_t*& buf_ptr, size_t i, 
    std::vector<Jump *>& jumps, bool conditional, size_t length)
{
    Jump *jump = new Jump;

    jump->ptr_from = buf_ptr;
    jump->ptr_to = nullptr;
    jump->cmd_from = &(cmds[i]);
    jump->cmd_to = &(cmds[cmds[i].arg1]);
    jump->conditional = conditional;
    jump->visible = true;
    jumps.push_back(jump);
    buf_ptr += length;
}

inline void add_invisible_jump(uint8_t* ptr_to, std::vector<Jump *>& jumps)
{
    Jump *jump = new Jump;
    memset(jump, 0, sizeof(Jump));

    jump->visible = false;
    jump->ptr_to = ptr_to;

    jumps.push_back(jump);
}