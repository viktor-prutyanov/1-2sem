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

int main()
{
    Stack_t *stack = Stack_ctor(3);

    Stack_dump(stack);

    Stack_push(stack, 2);
    Stack_push(stack, 3);
    Stack_push(stack, 5);
    Stack_push(stack, 7);

    Stack_dump(stack);

    printf("%lf\n", Stack_pop(stack));
    printf("%lf\n", Stack_pop(stack));

    Stack_dump(stack);

    Stack_dtor(stack);
    #ifdef _DEBUG
        system ("pause");
    #endif
    
    return 0;
}