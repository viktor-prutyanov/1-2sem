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
#include "cpu.h"

bool CPU_run_next(CPU_t *cpu)
{
    double val = 0;
    bool success = false;
    if (!(0 <= cpu->ip && cpu->ip < cpu->buffer_size))
    {
        cpu->is_end = true;
    }
    #define DEF_CMD(cmd, num, code, name, args)                     \
    else if (num == (cpu->buffer)[cpu->ip])                         \
    {                                                               \
        if (args == 1)                                              \
        {                                                           \
            if ((cpu->ip) != (cpu->buffer_size) - 1)                \
            {                                                       \
                val = (cpu->buffer)[(cpu->ip) + 1];                 \
                (cpu->ip)++;                                        \
            }                                                       \
        }                                                           \
        code;                                                       \
        val = 0;                                                    \
    }
    #include "..\include\commands.h"
    #undef DEF_CMD
    else
    {
        return false;
    }
    (cpu->ip)++;
}

bool CPU_load(CPU_t *cpu, FILE *in_file)
{
    fread ((void *)(cpu->buffer), sizeof(double), cpu->buffer_size, in_file);
    return true;
}

bool CPU_dump(CPU_t *cpu)
{
    if (CPU_ok(cpu))
    {
        printf ("Stack is OK, ax = %lf, ip = %d, buffer_size = %d\n", cpu->ax, cpu->ip, cpu->buffer_size );
        Stack_dump (cpu->stack);
        printf ("Buffer 0x%x\n", cpu->buffer);
        //for (int i = 0; i < cpu->buffer_size; i++)
        //{
        //    printf ("[%d] %lf\n", i, (cpu->buffer)[i]);
        //}
        return true;
    }
    else
    {
        printf ("Stack is NOT OK, ax = %lf, ip = %d, buffer_size = %d\n", cpu->ax, cpu->ip, cpu->buffer_size );
        Stack_dump (cpu->stack);
        return false;
    }
}

bool CPU_ok(CPU_t *cpu)
{
    return Stack_ok(cpu->stack) && (cpu->buffer != nullptr) && (cpu->ip < cpu->buffer_size);
}

bool CPU_ctor(CPU_t *cpu, size_t buffer_size, size_t stack_size)
{
    cpu->ip = 0;
    cpu->ax = 0;
    cpu->is_end = false;
    cpu->buffer_size  = buffer_size;

    cpu->buffer = (double *)calloc (buffer_size, sizeof (double));
    if (cpu->buffer == nullptr) return false;

    cpu->stack = (Stack_t*)calloc (1, sizeof (*cpu->stack));
    if (cpu->stack == nullptr)
    {
        free(cpu->buffer);
        cpu->buffer = nullptr;
        return false;
    }
    Stack_ctor (stack_size, cpu->stack);
    if (!Stack_ctor)
    {
        free(cpu->buffer);
        cpu->buffer = nullptr;
        free(cpu->stack);
        cpu->stack = nullptr;
        return false;
    }
    return Stack_ok(cpu->stack);
}

bool CPU_dtor(CPU_t *cpu)
{
    if (cpu == nullptr) return false;
    Stack_dtor(cpu->stack);
    cpu->stack = nullptr;
    free(cpu->buffer);
    cpu->buffer = nullptr;
    cpu->ax = 0;
    cpu->ip = 0;
    cpu->buffer_size= 0;
    return true;
}

bool CPU_push(Stack_t *stack, double value)
{
    return Stack_push (stack, value); 
} 

double CPU_pop(Stack_t *stack, bool *success)
{
    double result = Stack_pop (stack, success);
    if (!success)
    {
        return result;
    }
    else
    {
        return result;
    }
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

    return Stack_push (stack, arg1 - arg2);
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

bool CPU_out(Stack_t *stack)
{
    bool success = false;
    printf("%lf\n", Stack_pop(stack, &success));
    return success;
}

bool CPU_jmp(Stack_t *stack, int new_ip, int *ip, int length)
{
    if ((0 <= new_ip) && (new_ip < length))
    {
        (*ip) = new_ip - 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool CPU_je(Stack_t *stack, int value, int *ip, int length)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = CPU_pop (stack, &success);
    if (success) 
    {
        arg2 = CPU_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 == arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}

bool CPU_jne(Stack_t *stack, int value, int *ip, int length)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = CPU_pop (stack, &success);
    if (success) 
    {
        arg2 = CPU_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 != arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}

bool CPU_ja(Stack_t *stack, int value, int *ip, int length)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = CPU_pop (stack, &success);
    if (success) 
    {
        arg2 = CPU_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 > arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}

bool CPU_jae(Stack_t *stack, int value, int *ip, int length)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = CPU_pop (stack, &success);
    if (success) 
    {
        arg2 = CPU_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 >= arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}

bool CPU_jb(Stack_t *stack, int value, int *ip, int length)
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

    if (arg1 < arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}

bool CPU_jbe(Stack_t *stack, int value, int *ip, int length)
{
    double arg1 = 0;
    double arg2 = 0;
    bool success = false;

    arg1 = CPU_pop (stack, &success);
    if (success) 
    {
        arg2 = CPU_pop (stack, &success);
        if (!success) return false;
    }
    else
    {
        return false;
    }

    if (arg1 <= arg2) 
    {
        return CPU_jmp(stack, value, ip, length);
    }
    else
    {
        return true;
    }
}