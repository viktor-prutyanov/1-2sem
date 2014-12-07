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
#include <stdio.h>
#include "string.h"

#define CHECK_ERROR if (_err) { return 0; }
#define   SET_ERROR _err = true
#define RESET_ERROR _err = false

char   *_cur_sym = nullptr;
char *_start_sym = nullptr;
bool _err = false;

int GetN(TreeNode_t **node)
{
    CHECK_ERROR;
    char *n_start_sym = _cur_sym;
    int val = 0;
    while (true)
    {
        if ('0' <= *_cur_sym && *_cur_sym <= '9')
        {
            val = val * 10 + *_cur_sym++ - '0';
        }
        else if (*_cur_sym == '(' || *_cur_sym == ')' || *_cur_sym == '+' || *_cur_sym == '-' || *_cur_sym == '/' || *_cur_sym == '*' || *_cur_sym == '^' || *_cur_sym == 0)
        {
            break;
        }
        else
        {
            printf ("Syntax error: unknown symbol \"%c\" (code = %d). (Position = %d)\n", *_cur_sym, *_cur_sym, _cur_sym - _start_sym);
            SET_ERROR;
            return 0;
        }
    }
    if (_cur_sym - n_start_sym == 0)
    {
        printf ("Syntax error: number expected, nothing found. (Position = %d)\n", _cur_sym - _start_sym);
        SET_ERROR;
        return 0;
    }
    TreeData_t elem = {val, NUM};
    *node = TreeNode_new (elem, nullptr, nullptr);
    return val;
}

int GetX(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    int val1 = GetP (&node1);
    CHECK_ERROR;
    int val2 = 0;

    if (*_cur_sym == '^')
    {
        _cur_sym++;

        val2 = GetX (&node2);
        CHECK_ERROR;
        val1 = pow (val1, val2);

        TreeData_t elem = {'^', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return val1;
}

int GetP(TreeNode_t **node)
{
    CHECK_ERROR;

    int val = 0;
    if (*_cur_sym == '(')
    {
        _cur_sym++;

        val = GetE (node);
        CHECK_ERROR;

        if (*_cur_sym != ')')
        {
            SET_ERROR;
            printf ("Syntax error: \")\" expected, \"%c\" found. (Position = %d)\n", *_cur_sym, _start_sym - _cur_sym);
            return val;
        }
        _cur_sym++;
    }
    else
    {
        val = GetN (node);
    }
    return val;
}

int GetE(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    int val1 = GetT (&node1);
    CHECK_ERROR;
    int val2 = 0;

    if (*_cur_sym == '+')
    {
        _cur_sym++; 

        val2 = GetE (&node2);
        CHECK_ERROR;
        val1 += val2;

        TreeData_t elem = {'+', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (*_cur_sym == '-')
    {
        _cur_sym++;

        val2 = GetE (&node2);
        CHECK_ERROR;
        val1 -= val2;

        TreeData_t elem = {'-', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return val1;
}

int GetT(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    int val1 = GetX (&node1);
    CHECK_ERROR;
    int val2 = 0;

    if (*_cur_sym == '*')
    {
        _cur_sym++;

        val2 = GetT (&node2);
        CHECK_ERROR;
        val1 *= val2;

        TreeData_t elem = {'*', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (*_cur_sym == '/')
    {
        _cur_sym++;

        val2 = GetT (&node2);
        CHECK_ERROR;
        val1 /= val2;

        TreeData_t elem = {'/', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return val1;
}

int GetG0(char *expr, Tree_t *tree)
{
    RESET_ERROR;
    _cur_sym = expr;
    _start_sym = _cur_sym;
    int val = GetE (&(tree->root));
    CHECK_ERROR;
    if (strlen (expr) != (_cur_sym - _start_sym)) SET_ERROR;
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