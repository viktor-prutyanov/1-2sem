/**
*   Stack (LIFO) data structure realisation. 
*
*   @file main.c
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include <string.h>
#include <math.h>

#define ADD  1
#define SUB  2
#define MUL  3
#define DIV  4
#define SIN  5
#define COS  6
#define SQRT 7

#define SAFE_2ARG_OP(stack, OPERATION)  if (!two_arg_operation (stack, OPERATION)) { printf ("2 argument operation failed. See the dump below.\n"); Stack_dump (stack); }
#define SAFE_1ARG_OP(stack, OPERATION)  if (!one_arg_operation (stack, OPERATION)) { printf ("1 argument operation failed. See the dump below.\n"); Stack_dump (stack); }

/**
    @brief Operation with 2 arguments

    @param stack is pointer to stack
    @param operation is ADD, SUB, MUL or DIV

    @return true if success, false if not
*/
bool two_arg_operation(Stack_t *stack, int operation);

/**
    @brief Operation with 1 argument

    @param stack is pointer to stack
    @param operation is SIN, COS of SQRT

    @return true if success, false if not
*/
bool one_arg_operation(Stack_t *stack, int operation);

int main()
{
    char command[5];
    size_t size = 0;
    double arg = 0;
    bool success = false;

    printf ("Enter size of stack:\n");
    scanf ("%d", &size);
    Stack_t* stack = (Stack_t*) calloc(1, sizeof (*stack));
    Stack_ctor(size, stack);
    if (!Stack_ok(stack))
    {
        printf ("Stack construction failed.");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return 0;
    }

    printf ("Stack with size %d created.\n", size);

    while (true)
    {
        scanf ("%s", &command);
        if (strcmp (command, "push") == 0)
        {
            scanf ("%lf", &arg);
            if(!Stack_push (stack, arg)) 
            {
                printf ("Push failed. See the dump below.\n");
                Stack_dump (stack);
            }
        }
        else if (strcmp (command, "pop") == 0)
        {
            printf ("%lf\n", Stack_pop (stack, &success));
            if (!success)
            {
                printf ("Pop failed. See the dump below.\n");
                Stack_dump (stack);
            }
        }
        else if (strcmp (command, "add") == 0)
        {
            SAFE_2ARG_OP(stack, ADD);
        }
        else if (strcmp (command, "sub") == 0)
        {
            SAFE_2ARG_OP(stack, SUB);
        }
        else if (strcmp (command, "mul") == 0)
        {
            SAFE_2ARG_OP(stack, MUL);
        }
        else if (strcmp (command, "div") == 0)
        {
            SAFE_2ARG_OP(stack, DIV);
        }
        else if (strcmp (command, "sin") == 0)
        {
            SAFE_1ARG_OP(stack, SIN);
        }
        else if (strcmp (command, "cos") == 0)
        {
            SAFE_1ARG_OP(stack, COS);
        }
        else if (strcmp (command, "sqrt") == 0)
        {
            SAFE_1ARG_OP(stack, SQRT);
        }
        else if (strcmp (command, "dump") == 0)
        {
            Stack_dump (stack);
        }
        else if (strcmp (command, "ok") == 0)
        {
            printf ("%d\n", Stack_ok(stack));
        }
        else if (strcmp (command, "stop") == 0)
        {
            Stack_dtor(stack);
            break;
        }
        else
        {
            printf ("%s : unknown command\n", command);
        }
    }

    #ifdef _DEBUG
        system ("pause");
    #endif
    
    return 0;
}

bool two_arg_operation(Stack_t *stack, int operation)
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

    switch (operation)
    {
        case ADD:
            success = Stack_push (stack, arg1 + arg2);
            break;
        case SUB:
            success = Stack_push (stack, arg2 - arg1);
            break;
        case MUL:
            success = Stack_push (stack, arg1 * arg2);
            break;
        case DIV:
            success = Stack_push (stack, arg2 / arg1);
            break;
        default:
            return false;
            break;
    }

    return true;
}

bool one_arg_operation(Stack_t *stack, int operation)
{
    double arg = 0;
    bool success = false;

    arg = Stack_pop(stack, &success);

    if (!success)
    {
        return false;
    }

    switch (operation)
    {
        case SIN:
            success = Stack_push (stack, sin(arg));
            break;
        case COS:
            success = Stack_push (stack, cos(arg));
            break;
        case SQRT:
            success = Stack_push (stack, sqrt(arg));
            break;
        default:
            return false;
            break;
    }
    
    return true;
}