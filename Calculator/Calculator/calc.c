/**
*   Expression calculator
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "calc.h"

char *_cur_sym;

int GetN() //N ::= [0-9]+0
{
    int val = 0;
    while ('0' <= *_cur_sym && *_cur_sym <= '9')
    {
        val = val * 10 + *_cur_sym++ - '0';

    }
    return val;
}

int GetX()
{
    int val = GetP();
    while (*_cur_sym == '^')
    {
        _cur_sym++;
        val = pow(val, GetP());
    }
    return val;
}

int GetP() //P ::= "("E")"|N
{
    if (*_cur_sym == '(')
    {
        _cur_sym++;
        int val = GetE();
        _cur_sym++;
        return val;
    }
    else
    {
        return GetN();
    }
}

int GetE() //E ::= T{"+-"T}*
{
    int val = GetT();
    while (*_cur_sym == '+')
    {
        _cur_sym++;
        val += GetT();
    }
    while (*_cur_sym == '-')
    {
        _cur_sym++;
        val -= GetT();
    }
    return val;
}

int GetT() //T ::= P{"*/"P}*
{
    int val = GetX();
    while (*_cur_sym == '*')
    {
        _cur_sym++;
        val *= GetX();
    }
    while (*_cur_sym == '/')
    {
        _cur_sym++;
        val /= GetX();
    }
    return val;
}

int GetG0(char *expr) //G0 ::= E
{
    _cur_sym = expr;
    int val = GetE();
    return val;
}

unsigned int pow(unsigned int base, unsigned int power)
{
    unsigned int result = 1;
    while (power != 0) 
    {
        if (power & 1)
        {
            power--;
            result *= base;
        }
        else
        {
            power >>= 1;
            base *= base;
        }
        
    }
    return result;
}