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
#define   SET_ERROR { Err = true; return 1;}
#define RESET_ERROR Err = false

#define CHECK_OPENING_BRACKET       if (Cur_token->value != '(' || Cur_token->type != CONTROL) { SET_ERROR; }
#define CHECK_CLOSING_BRACKET       if (Cur_token->value != ')' || Cur_token->type != CONTROL) { SET_ERROR; }

#define CHECK_OPENING_CURLY_BRACKET if (Cur_token->value != '{' || Cur_token->type != CONTROL) { SET_ERROR; }
#define CHECK_CLOSING_CURLY_BRACKET if (Cur_token->value != '}' || Cur_token->type != CONTROL) { SET_ERROR; }

#define CHECK_SEMICOLON             if (Cur_token->value != ';' || Cur_token->type != CONTROL) { SET_ERROR; }

Token_t *Cur_token = nullptr;
bool Err           = false;

int GetN(TreeNode_t **node)
{
    CHECK_ERROR;

    int sign = 1;

    if (Cur_token->type == A_OPERATOR && Cur_token->value == '-')
    {
        sign = -1;
        Cur_token++;
    }

    if (Cur_token->type == NUMBER || Cur_token->type == VARIABLE)
    {
        TreeData_t elem = {(Cur_token->value) * sign, Cur_token->type};
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

    if (Cur_token->value == '^' && Cur_token->type == A_OPERATOR)
    {
        Cur_token++;

        GetX (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'^', A_OPERATOR};
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
        CHECK_CLOSING_BRACKET;
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

    if (Cur_token->value == '+' && Cur_token->type == A_OPERATOR)
    {
        Cur_token++; 

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'+', A_OPERATOR};
        *node = TreeNode_new (elem, node1, node2);

    }
    else if (Cur_token->value == '-' && Cur_token->type == A_OPERATOR)
    {
        Cur_token++;

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'-', A_OPERATOR};
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

    if (Cur_token->value == '*' && Cur_token->type == A_OPERATOR)
    {
        Cur_token++;

        GetT (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'*', A_OPERATOR};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (Cur_token->value == '/' && Cur_token->type == A_OPERATOR)
    {
        Cur_token++;

        GetT (&node2);
        CHECK_ERROR

        TreeData_t elem = {'/', A_OPERATOR};
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
        if (Cur_token->value == '=' && Cur_token->type == L_OPERATOR)
        {
            Cur_token++;
            GetE (&node2);
            CHECK_ERROR;
            TreeData_t elem = {'=', L_OPERATOR};
            *node = TreeNode_new (elem, node1, node2);
        }
        else
        {
            SET_ERROR;
        }

        CHECK_SEMICOLON;
        Cur_token++;
    }
    else if (Cur_token->type == W_OPERATOR)
    {
        GetW (node);

        CHECK_ERROR;
    }
    else
    {
        GetF (node);
        CHECK_ERROR;

        CHECK_SEMICOLON;
        Cur_token++;
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
    if (!(Cur_token->type == CONTROL && Cur_token->value == '}'))
    {
        GetB (&node2);
        CHECK_ERROR;
    }
    else
    {
        Cur_token++;
    }
    *node = TreeNode_new (elem, node1, node2);

    return 0;
}

int GetQ(TreeNode_t **node)
{
    CHECK_ERROR;

    CHECK_OPENING_CURLY_BRACKET;
    Cur_token++;

    GetB (node);
    CHECK_ERROR;

    return 0;
}

int GetBE(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    GetE (&node1);
    CHECK_ERROR;

    if (Cur_token->type != L_OPERATOR) SET_ERROR;
    TreeData_t elem = {Cur_token->value, L_OPERATOR};

    Cur_token++;
    GetE (&node2);
    CHECK_ERROR;

    *node = TreeNode_new (elem, node1, node2); 
    return 0;
}

int GetW(TreeNode_t **node)
{
    CHECK_ERROR;

    TreeNode_t *node1 = nullptr;
    TreeNode_t *node2 = nullptr;

    if (Cur_token->type != W_OPERATOR) SET_ERROR;
    TreeData_t elem = {Cur_token->value, W_OPERATOR};
    Cur_token++;

    CHECK_OPENING_BRACKET;
    Cur_token++;

    GetBE (&node1);
    CHECK_ERROR;

    CHECK_CLOSING_BRACKET;
    Cur_token++;

    GetQ (&node2);
    CHECK_ERROR;

    *node = TreeNode_new (elem, node1, node2);

    return 0;
}

int GetF(TreeNode_t **node)
{
    CHECK_ERROR;

    if (Cur_token->type != FUNCTION ) SET_ERROR;
    TreeData_t elem = {Cur_token->value, FUNCTION};
    Cur_token++;

    CHECK_OPENING_BRACKET;
    Cur_token++;
    GetE(node);
    CHECK_ERROR;
    *node = TreeNode_new (elem, nullptr, *node); 
    CHECK_CLOSING_BRACKET;
    Cur_token++;

    return 0;
}

int GetG0(Token_t *tokens, Tree_t *tree, int tokens_amount)
{
    RESET_ERROR;
    Cur_token = tokens;
    GetQ (&(tree->root));
    CHECK_ERROR;
    return 0;
}