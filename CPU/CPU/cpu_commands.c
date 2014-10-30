#include <stdio.h>
#include "cpu_commands.h"

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