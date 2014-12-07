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
    char expr[MAX_EXPR_SIZE] = {};
    printf ("Enter expression (max length is %d) you want to calculate, you can use +-/*^()\n", MAX_EXPR_SIZE);
    scanf ("%s", &expr);
    
    Tree_t *tree = (Tree_t *)calloc (1, sizeof(Tree_t));
    Tree_ctor (tree);
    
    int result = GetG0 (expr, tree);
    extern bool _err;
    if (_err)
    {
        printf ("There was an error. Calculation aborted.\n");
    }
    else
    {
        printf ("Result is : %d\n", result);
        Tree_print (tree, stdout);
        printf ("\n");
    }

    Tree_dtor (tree);
    free (tree);
    tree = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
}