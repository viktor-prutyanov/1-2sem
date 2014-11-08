/**
*   CPU commands
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"

#define ARG1 ((cpu->commands[cpu->ip]).arg1)
#define ARG2 ((cpu->commands[cpu->ip]).arg2)
#define ARG3 ((cpu->commands[cpu->ip]).arg3)

#define IS_JUMP(num) ((10 <= num) && (num <= 18) || (num == 21))

bool CPU_run_next(CPU_t *cpu)
{
    double *arg1, *arg2, *arg3;
    uint16_t jump_to = 0;
    bool success = false;
    if (0 > cpu->ip || cpu->ip >= cpu->commands_size)
    {
        cpu->is_end = true;
    }
    #define DEF_CMD(cmd, num, code, name, args)             \
    else if (num == (cpu->commands[cpu->ip]).number)        \
    {                                                       \
        if (args == 1)                                      \
        {                                                   \
            if IS_JUMP(num)                                 \
            {                                               \
                jump_to = ARG1;                             \
            }                                               \
            else                                            \
            {                                               \
                SET_ARG(cpu, ARG1, arg1);                   \
            }                                               \
        }                                                   \
        else if (args == 2)                                 \
        {                                                   \
            SET_ARG(cpu, ARG1, arg1);                       \
            SET_ARG(cpu, ARG2, arg2);                       \
        }                                                   \
        code;                                               \
        (cpu->ip)++;                                        \
    }
    #include "..\include\commands.h"
    #undef DEF_CMD
    else
    {
        return false;
    }
    return true;
}

bool CPU_load(CPU_t *cpu, size_t values_size, size_t commands_size, FILE *in_file)
{    
    fread ((void *)(cpu->values), sizeof(double), cpu->values_size, in_file);
    fread ((void *)(cpu->commands), sizeof(cmd_t), cpu->commands_size, in_file);
    return true;
}

bool CPU_dump(CPU_t *cpu)
{
    if (CPU_ok(cpu))
    {
        printf ("CPU is OK, ax = %lf, bx = %lf, cx = %lf, dx = %lf, ip = %d, values_size = %d, commands_size = %d\n\nStack:\n", cpu->ax, cpu->bx, cpu->cx, cpu->dx, cpu->ip, cpu->values_size, cpu->commands_size );
        Stack_dump (cpu->stack);
        printf ("Call stack:\n");
        Stack_dump (cpu->call_stack);
        printf ("\n");

        printf ("Values [0x%x]\n", cpu->values);
        for (int i = 0; i < cpu->values_size; i++)
        {
            printf ("\t[%3d] %lf\n", i, (cpu->values)[i]);
        }
        printf ("\n");

        printf ("Commands [0x%x]\n", cpu->commands);
        for (int i = 0; i < cpu->commands_size; i++)
        {
            printf ("\t[%3d] %3d", i, ((cpu->commands)[i]).number);
            PRINT_ARG(arg1);
            PRINT_ARG(arg2);
            PRINT_ARG(arg3);
            printf ("\n");
        }
        return true;
    }
    else
    {
        printf ("CPU is NOT OK, ax = %lf, bx = %lf, cx = %lf, dx = %lf, ip = %d, values_size = %d, commands_size = %d\n", cpu->ax, cpu->bx, cpu->cx, cpu->dx, cpu->ip, cpu->values_size, cpu->commands_size );
        Stack_dump (cpu->stack);
        Stack_dump (cpu->call_stack);
        return false;
    }
}

bool CPU_ok(CPU_t *cpu)
{
    return Stack_ok(cpu->stack) && Stack_ok(cpu->call_stack) && (cpu->values != nullptr) && (cpu->commands != nullptr) && (cpu->ip < cpu->commands_size);
}

bool CPU_ctor(CPU_t *cpu, size_t values_size, size_t commands_size, size_t stack_size, size_t call_stack_size)
{
    cpu->ip = 0;

    cpu->ax = 0;
    cpu->bx = 0;
    cpu->cx = 0;
    cpu->dx = 0;

    cpu->is_end = false;

    cpu->commands_size  = commands_size;
    cpu->values_size = values_size;

    cpu->commands = (cmd_t *)calloc (commands_size, sizeof (cmd_t));
    cpu->values = (double *)calloc (values_size, sizeof (double));
    if ((cpu->commands == nullptr) || (cpu->values == nullptr)) return false;

    cpu->stack = (Stack_t*)calloc (1, sizeof (*cpu->stack));
    if (cpu->stack == nullptr)
    {
        free(cpu->values);
        free(cpu->commands);
        cpu->values = nullptr;
        cpu->commands = nullptr;
        return false;
    }
    cpu->call_stack = (Stack_t*)calloc (1, sizeof (*cpu->call_stack));    
    if (cpu->call_stack == nullptr)
    {
        free(cpu->stack);
        free(cpu->values);
        free(cpu->commands);
        cpu->stack = nullptr;
        cpu->values = nullptr;
        cpu->commands = nullptr;
        return false;
    }

    if (!(Stack_ctor (stack_size, cpu->stack) && Stack_ctor (call_stack_size, cpu->call_stack)))
    {
        free(cpu->stack);
        free(cpu->values);
        free(cpu->commands);
        cpu->stack = nullptr;
        cpu->values = nullptr;
        cpu->commands = nullptr;
        return false;
    }
    return Stack_ok(cpu->stack) && Stack_ok(cpu->call_stack);
}

bool CPU_dtor(CPU_t *cpu)
{
    if (cpu == nullptr) return false;

    Stack_dtor (cpu->stack);
    Stack_dtor (cpu->call_stack);
    cpu->stack = nullptr;
    cpu->call_stack = nullptr;

    free (cpu->commands);
    free (cpu->values);
    cpu->commands = nullptr;
    cpu->values = nullptr;

    cpu->ax = 0;
    cpu->ax = 0;
    cpu->ax = 0;
    cpu->ax = 0;
    cpu->ip = 0;
    cpu->commands_size = 0;
    cpu->values_size = 0;
    cpu->is_end = false;

    return true;
}

bool CPU_push(Stack_t *stack, double value)
{
    return Stack_push (stack, value); 
} 

bool CPU_pop(Stack_t *stack, double *dest)
{
    bool success = false;
    *dest = Stack_pop (stack, &success);
    return success;
}

bool CPU_mul(Stack_t *stack)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    return Stack_push (stack, arg1 * arg2);
}

bool CPU_add(Stack_t *stack)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    return Stack_push (stack, arg1 + arg2);
}

bool CPU_sub(Stack_t *stack)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    return Stack_push (stack, arg2 - arg1);
}

bool CPU_div(Stack_t *stack)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    return Stack_push (stack, arg1 / arg2);
}

bool CPU_out(CPU_t *cpu)
{
    bool success = false;
    printf("->%lf\n", Stack_pop(cpu->stack, &success));
    return success;
}

bool CPU_jmp(CPU_t *cpu, int jump_to)
{
    if ((0 <= jump_to) && (jump_to < cpu->commands_size) && (cpu != nullptr))
    {
        (cpu->ip) = jump_to - 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool CPU_je( CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 == arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jne(CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 != arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_ja( CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 > arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jae(CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 >= arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jb( CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 < arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jbe(CPU_t *cpu, int jump_to)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        arg2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 <= arg2) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jz( CPU_t *cpu, int jump_to)
{
    bool success = false;
    double arg1 = Stack_pop (cpu->stack, &success);
    if (!success) return false;

    if (arg1 == 0) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_jnz(CPU_t *cpu, int jump_to)
{
    bool success = false;
    double arg1 = Stack_pop (cpu->stack, &success);
    if (!success) return false;

    if (arg1 != 0) 
    {
        return CPU_jmp(cpu, jump_to);
    }
    else
    {
        return true;
    }
}

bool CPU_mov(double *dest, double src)
{
    if (dest == nullptr) return false;
    *dest = src;
    return true;
}

bool CPU_swap(CPU_t *cpu)
{
    double swap1 = 0;
    double swap2 = 0;
    bool success = false;

    swap1 = Stack_pop (cpu->stack, &success);
    if (success) 
    {
        swap2 = Stack_pop (cpu->stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    return Stack_push (cpu->stack, swap1) && Stack_push (cpu->stack, swap2);
}

bool CPU_call(CPU_t *cpu,  int jump_to)
{
    if (cpu == nullptr) return false;
    Stack_push (cpu->call_stack, cpu->ip);
    //printf ("Call from %d to %d\n", cpu->ip, jump_to);
    return CPU_jmp (cpu, jump_to); 
}

bool CPU_ret(CPU_t *cpu)
{
    if (cpu == nullptr) return false;
    bool success = false;
    int jump_to = Stack_pop (cpu->call_stack, &success);
    if (!success) return false;
    //printf ("Return from %d to %d\n", cpu->ip, jump_to + 1);
    return CPU_jmp (cpu, jump_to + 1);
}

bool CPU_dup(CPU_t *cpu)
{
    if (cpu == nullptr) return false;
    double val = 0;
    bool success = false;

    val = Stack_peak (cpu->stack, &success);
    if (!success) return false; 

    return Stack_push (cpu->stack, val);
}

bool CPU_in(CPU_t *cpu)
{
    if (cpu == nullptr) return false;
    double val = 0;
    printf ("<-");
    scanf ("%lf", &val);
    return Stack_push (cpu->stack, val);
}