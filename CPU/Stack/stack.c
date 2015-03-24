/**
*   Stack (LIFO) data structure realisation. 
*
*   @file stack.c
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "stack.h"
#include <assert.h>

bool Stack_ctor(size_t size, Stack_t* stack)
{
    if (stack == nullptr) return false;
    stack->size = size;
    stack->count = 0;
    stack->data = (element_t *)calloc (size, sizeof (element_t));
    return !(stack->data == nullptr);
}

bool Stack_dtor(Stack_t *stack)
{
    if (stack == nullptr) return false;
    free (stack->data);
    stack->data = nullptr;
    stack->count = 0;
    stack->size = 0;
    free (stack);
    stack = nullptr;
    return true;
}

bool Stack_push(Stack_t *stack, element_t value)
{
    VERIFY (stack);
    if (!Stack_ok (stack)) return false;
    if (Stack_full (stack)) 
    {
        stack->data = (element_t *)realloc ((void *)(stack->data), (stack->size + 1) * sizeof (element_t));
        if (!Stack_ok (stack)) return false;
        if (stack->size != SIZE_MAX)
        {
            stack->size++;
        }
        else
        {
            return false;
        }
    }
    (stack->data)[stack->count] = value;
    if (stack->count != SIZE_MAX)
    {
        stack->count++;
    }
    else
    {
        return false;
    }
    VERIFY (stack);
    return Stack_ok (stack);
}

element_t Stack_pop(Stack_t *stack, bool *success)
{
    VERIFY (stack);
    if (Stack_ok (stack) && !Stack_empty (stack))
    {
        (stack->count)--;
        *success = true;
        VERIFY (stack);
        return (stack->data)[stack->count];
    }
    else
    {
        *success = false;
        VERIFY (stack);
        return 0;
    }
}

bool Stack_ok(Stack_t *stack)
{
    return !((stack == nullptr) || (stack->count < 0) || (stack->count > stack->size) || (stack->data == nullptr));
}

bool Stack_full(Stack_t *stack)
{
    return (stack->count == stack->size);
}

bool Stack_empty(Stack_t *stack)
{
    return (stack->count == 0);
}

bool Stack_dump(Stack_t *stack)
{
    if (stack == nullptr)
    {
        fprintf (stdout, "Dump failed.\n");

        return false;
    }
    else
    {
        if (Stack_ok(stack))
        {
            fprintf (stdout, "Stack [0x%x] is OK. \n\tcount = %d\n\tsize = %d\n\tdata [0x%x]:\n", stack, stack->count, stack->size, stack->data);
            for (int i = 0; i < stack->count; i++)
            {
                printf("\t[%d] %lf\n", i, (stack->data)[i]);
            }
        }
        else if (!Stack_ok(stack))
        {
            fprintf (stdout, "Stack [0x%x] is NOT OK. \n\tcount = %d\n\tsize = %d\n\tdata [0x%x]:\n", stack, stack->count, stack->size, stack->data);
        }

        return true;
    }
}

size_t Stack_get_size(Stack_t *stack)
{
    return stack->size;
}

size_t Stack_get_count(Stack_t *stack)
{
    return stack->count;
}

element_t Stack_peak(Stack_t *stack, bool *success)
{
    VERIFY (stack);
    if (Stack_ok (stack) && !Stack_empty (stack))
    {
        *success = true;
        VERIFY (stack);
        return (stack->data)[(stack->count)-1];
    }
    else
    {
        *success = false;
        VERIFY (stack);
        return 0;
    }
}