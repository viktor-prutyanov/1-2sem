/**
*   CPU
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "..\Stack\stack.h"

typedef struct CPU_t 
{
    Stack_t *stack;
    double  ax;
    size_t  buffer_size;
    double  *buffer;
    int     ip;
    bool    is_end;
};


bool    CPU_dump        (CPU_t *cpu);
bool    CPU_ok          (CPU_t *cpu);
bool    CPU_load        (CPU_t *cpu, FILE *in_file);
bool    CPU_ctor        (CPU_t *cpu, size_t buffer_size, size_t stack_size);
bool    CPU_dtor        (CPU_t *cpu);
bool    CPU_run_next    (CPU_t *cpu);

bool    CPU_push        (Stack_t *stack, double value);
double  CPU_pop         (Stack_t *stack, bool *success);

bool    CPU_mul         (Stack_t *stack);
bool    CPU_div         (Stack_t *stack);
bool    CPU_add         (Stack_t *stack);
bool    CPU_sub         (Stack_t *stack);

bool    CPU_out         (Stack_t *stack);

bool    CPU_jmp         (Stack_t *stack, int value, int *ip, int length);
bool    CPU_je          (Stack_t *stack, int value, int *ip, int length);
bool    CPU_jne         (Stack_t *stack, int value, int *ip, int length);
bool    CPU_ja          (Stack_t *stack, int value, int *ip, int length);
bool    CPU_jae         (Stack_t *stack, int value, int *ip, int length);
bool    CPU_jb          (Stack_t *stack, int value, int *ip, int length);
bool    CPU_jbe         (Stack_t *stack, int value, int *ip, int length);