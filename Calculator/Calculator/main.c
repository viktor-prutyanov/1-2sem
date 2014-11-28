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

int main()
{
    char str[256];
    scanf ("%s", &str);
    printf ("%d\n", GetG0(str));

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
}