/**
*   CPU
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdint.h>
#include "..\Stack\stack.h"

#define NO_CMD INT16_MIN

#define PRINT_ARG(arg) ((cpu->commands)[i]).##arg == NO_CMD ? printf(" NO_CMD") : printf("%7d", ((cpu->commands)[i]).##arg)

#define SET_ARG(cpu, ARG, arg)                 \
{                                              \
    if (ARG >= 0)                              \
    {                                          \
        arg = &(cpu->values[ARG]);             \
    }                                          \
    else if (ARG == -1)                        \
    {                                          \
        arg = &(cpu->ax);                      \
    }                                          \
    else if (ARG == -2)                        \
    {                                          \
        arg = &(cpu->bx);                      \
    }                                          \
    else if (ARG == -3)                        \
    {                                          \
        arg = &(cpu->cx);                      \
    }                                          \
    else if (ARG == -4)                        \
    {                                          \
        arg = &(cpu->dx);                      \
    }                                          \
    else                                       \
    {                                          \
        return false;                          \
    }                                          \
}

typedef struct cmd_t
{
    uint16_t number;
    int16_t arg1;
    int16_t arg2;
    int16_t arg3;
};

typedef struct CPU_t 
{
    Stack_t *stack;
    Stack_t *call_stack;

    double  ax, bx, cx, dx;

    cmd_t   *commands;
    size_t  commands_size;
    double  *values;
    size_t  values_size;

    int     ip;

    bool    is_end;
};


bool    CPU_dump        (CPU_t *cpu);
bool    CPU_ok          (CPU_t *cpu);
bool    CPU_load        (CPU_t *cpu, size_t values_size, size_t commands_size, FILE *in_file);
bool    CPU_ctor        (CPU_t *cpu, size_t values_size, size_t commands_size, size_t stack_size, size_t call_stack_size);
bool    CPU_dtor        (CPU_t *cpu);
bool    CPU_run_next    (CPU_t *cpu);

bool    CPU_push        (Stack_t *stack, double value);
bool    CPU_pop         (Stack_t *stack, double *dest);

bool    CPU_mul         (Stack_t *stack);
bool    CPU_div         (Stack_t *stack);
bool    CPU_add         (Stack_t *stack);
bool    CPU_sub         (Stack_t *stack);

bool    CPU_out         (CPU_t *cpu);

bool    CPU_jmp         (CPU_t *cpu, int jump_to);
bool    CPU_je          (CPU_t *cpu, int jump_to);
bool    CPU_jne         (CPU_t *cpu, int jump_to);
bool    CPU_ja          (CPU_t *cpu, int jump_to);
bool    CPU_jae         (CPU_t *cpu, int jump_to);
bool    CPU_jb          (CPU_t *cpu, int jump_to);
bool    CPU_jbe         (CPU_t *cpu, int jump_to);
bool    CPU_jz          (CPU_t *cpu, int jump_to);
bool    CPU_jnz         (CPU_t *cpu, int jump_to);

bool    CPU_mov         (double *dest, double src);

bool    CPU_swap        (CPU_t *cpu);

bool    CPU_call        (CPU_t *cpu,  int jump_to);
bool    CPU_ret         (CPU_t *cpu);

bool    CPU_dup         (CPU_t *cpu);

bool    CPU_in          (CPU_t *cpu);