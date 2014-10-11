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

//#define LOG

typedef struct Stack_t
{
    double *data;
    size_t count;
    size_t size;
};

Stack_t *Stack_ctor(size_t size)
{
    Stack_t* stack = (Stack_t*) calloc(1, sizeof (Stack_t));
    if (stack == nullptr) return nullptr;
    stack->size = size;
    stack->count = 0;
    stack->data = (double *)calloc (size, sizeof (double));

    if (stack->data == nullptr)
    {
        return nullptr;
    }
    else
    {
        return stack;
    }
}

bool Stack_dtor(Stack_t *stack)
{
    if (stack != nullptr)
    {
        free (stack->data);
        stack->data = nullptr;
        stack->count = 0;
        stack->size = 0;
        free (stack);
        stack = nullptr;
        return true;
    }
    else
    {
        return false;
    }
}

bool Stack_push(Stack_t *stack, double value)
{
    if (!Stack_ok (stack)) return true;
    if (Stack_full (stack)) 
    {
        stack->data = (double*)realloc ((void *)(stack->data), (stack->size + 1) * sizeof (double));
        if (!Stack_ok (stack)) return true;
        stack->size++;
    }
    (stack->data)[stack->count] = value;
    (stack->count)++;
    return !Stack_ok (stack);
}

double Stack_pop(Stack_t *stack, bool *success)
{
    if (Stack_ok (stack) && !Stack_empty (stack))
    {
        (stack->count)--;
        *success = true;
        return (stack->data)[stack->count];
    }
    else
    {
        *success = false;
        return 0;
    }
}

bool Stack_ok(Stack_t *stack)
{
    if ((stack == nullptr) || (stack->count < 0) || (stack->count > stack->size) || (stack->data == nullptr)) return false;
    return true;
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
    #ifdef LOG
        FILE *file = fopen("stack.log", "a");
        #define OUT (file)
    #else
        #define OUT (stdout) 
    #endif

    if (stack == nullptr)
    {
        #ifdef LOG
            fclose(file);
            file = nullptr;
        #endif

        fprintf (OUT, "Dump failed.\n");

        return false;
    }
    else
    {
        if (Stack_ok(stack))
        {
            fprintf (OUT, "Stack [0x%x] is OK. \n\tcount = %d\n\tsize = %d\n\tdata [0x%x]:\n", stack, stack->count, stack->size, stack->data);
            for  (size_t i = 0; i < (stack->count); i++)
            {
                fprintf(OUT, "\t[%d] [0x%x] %lf\n", i, &((stack->data)[i]), (stack->data)[i]);
            }
        }
        else if (!Stack_ok(stack))
        {
            fprintf (OUT, "Stack [0x%x] is NOT OK. \n\tcount = %d\n\tsize = %d\n\tdata [0x%x]:\n", stack, stack->count, stack->size, stack->data);
        }

        #ifdef LOG
            fclose(file);
            file = nullptr;
        #endif

        return true;
    }
}