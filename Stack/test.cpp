/**
*   Test.
*
*   @file test.cpp
*
*   @date 04.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

//TODO: Add comments.

#include <iostream>
#include <stdexcept>

#include "Expression.h"

#define MAX_EXPR_LEN 256

int main()
{
    char *infixStr = (char *)calloc(MAX_EXPR_LEN, sizeof(char));
    fgets(infixStr, MAX_EXPR_LEN, stdin);
    infixStr[strlen(infixStr) - 1] = '\0';

    Expression expr(infixStr);
    printf("Infix expression is {%s} with length %lu\n", 
        expr.InfixStr(), expr.GetInfixStrLen());

    if (expr.Convert())
    {
        printf("Postfix expression: %s\n", expr.PostfixStr());
    }
    else
    {
        //expr.~Expression();
        free(infixStr);
        return EXIT_FAILURE;
    }
    

    free(infixStr);

    return EXIT_SUCCESS;
}