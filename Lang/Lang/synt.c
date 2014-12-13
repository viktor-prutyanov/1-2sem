/**
*   Syntax analyzer
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "synt.h"
#include <stdio.h>
#include "string.h"
#include <ctype.h>
#include <stdlib.h>

#define CHECK_ERROR if (Err) { return 0; }
#define   SET_ERROR Err = true
#define RESET_ERROR Err = false

#define MAX_FUNC_NAME_LENGTH 8

Token_t *Cur_token = nullptr;
bool Err           = false;

int GetN(TreeNode_t **node)
{
    CHECK_ERROR;
    if (Cur_token->type == NUMBER || Cur_token->type == VARIABLE)
    {
        TreeData_t elem = {Cur_token->value, Cur_token->type};
        *node = TreeNode_new (elem, nullptr, nullptr);
        Cur_token++;
    }
    else
    {
        SET_ERROR;
    }
    return 0;
}

int GetX(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    GetP (&node1);
    CHECK_ERROR;

    if (Cur_token->value == '^' && Cur_token->type == OPERATOR)
    {
        Cur_token++;

        GetX (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'^', OPERATOR};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return 0;
}

int GetP(TreeNode_t **node)
{
    CHECK_ERROR;

    if (Cur_token->type == FUNCTION)
    {
        GetF (node);
        CHECK_ERROR;
    }
    else if (Cur_token->value == '(' && Cur_token->type == CONTROL)
    {
        Cur_token++;
        GetE (node);
        CHECK_ERROR;

        if (Cur_token->value != ')' || Cur_token->type != CONTROL)
        {
            SET_ERROR;
            return 0;
        }
        Cur_token++;
    }
    else
    {
        GetN (node);
    }
    return 0;
}

int GetE(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    GetT (&node1);
    CHECK_ERROR;

    if (Cur_token->value == '+' && Cur_token->type == OPERATOR)
    {
        Cur_token++; 

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'+', OPERATOR};
        *node = TreeNode_new (elem, node1, node2);

    }
    else if (Cur_token->value == '-' && Cur_token->type == OPERATOR)
    {
        Cur_token++;

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'-', OPERATOR};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return 0;
}

int GetT(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    GetX (&node1);
    CHECK_ERROR;

    if (Cur_token->value == '*' && Cur_token->type == OPERATOR)
    {
        Cur_token++;

        GetT (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'*', OPERATOR};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (Cur_token->value == '/' && Cur_token->type == OPERATOR)
    {
        Cur_token++;

        GetT (&node2);
        CHECK_ERROR

        TreeData_t elem = {'/', OPERATOR};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return 0;
}

int GetS(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    if (Cur_token->type == VARIABLE)
    {
        TreeData_t elem1 = {Cur_token->value, VARIABLE};
        node1 = TreeNode_new (elem1, nullptr, nullptr);
        Cur_token++;
        if (Cur_token->value == '=' && Cur_token->type == OPERATOR)
        {
            Cur_token++;
            GetE (&node2);
            CHECK_ERROR;
            TreeData_t elem = {'=', OPERATOR};
            *node = TreeNode_new (elem, node1, node2);
        }
        else
        {
            SET_ERROR;
        }
    }
    else
    {
        GetP (node);
        CHECK_ERROR;
    }
    if (Cur_token->value != ';' || Cur_token->type != CONTROL)
    {
        SET_ERROR;
    }
    return 0;
}

int GetB(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;
    TreeData_t elem = {'~', CONTROL};

    GetS (&node1);
    CHECK_ERROR;
    Cur_token++;
    if (Cur_token->type != CONTROL && Cur_token->value != '}')
    {
        GetB (&node2);
        CHECK_ERROR;
    }
    *node = TreeNode_new (elem, node1, node2);

    return 0;
}

int GetQ(TreeNode_t **node)
{
    CHECK_ERROR;

    if (Cur_token->type != CONTROL || Cur_token->value != '{')
    {
        SET_ERROR;
    }

    Cur_token++;
    GetB (node);
    CHECK_ERROR;

    return 0;
}

int GetA(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    if (Cur_token->type == FUNCTION)
    {
        Cur_token++;
        GetF (&node1);
        CHECK_ERROR;
        Cur_token++;
        GetQ (&node2);
        CHECK_ERROR;
        TreeData_t elem = {'$', CONTROL};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        Cur_token++;
        GetS (node);
        CHECK_ERROR;
    }
    return 0;
}

int GetF(TreeNode_t **node)
{
    TreeData_t elem = {Cur_token->value, FUNCTION};
    Cur_token++;
    if (Cur_token->value != '(' || Cur_token->type != CONTROL)
    {
        SET_ERROR;
    }
    Cur_token++;
    GetE(node);
    CHECK_ERROR;
    *node = TreeNode_new (elem, nullptr, *node); 

    if (Cur_token->value != ')' || Cur_token->type != CONTROL)
    {
        SET_ERROR;
        return 0;
    }
    Cur_token++;
}

int GetG0(Token_t *tokens, Tree_t *tree, int tokens_amount)
{
    RESET_ERROR;
    Cur_token = tokens;
    GetQ (&(tree->root));
    CHECK_ERROR;
    return 0;
}