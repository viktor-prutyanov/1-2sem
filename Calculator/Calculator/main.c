/**
*   Expression calculator
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

#define MAX_EXPR_SIZE 256

int main()
{
    char expr[MAX_EXPR_SIZE];
    printf ("Enter expression you want to calculate, you can use +-/*^()\n");
    scanf ("%s", &expr);
    printf ("%d\n", GetG0(expr));

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
}