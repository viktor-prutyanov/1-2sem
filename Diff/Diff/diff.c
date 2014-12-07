/**
*   Expression calculator
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "diff.h"
#include <stdio.h>
#include "string.h"
#include <ctype.h>
#include <stdlib.h>

#define CHECK_ERROR if (Err) { return 0; }
#define   SET_ERROR Err = true
#define RESET_ERROR Err = false

#define MAX_FUNC_NAME_LENGTH 8

char   *Cur_sym = nullptr;
char *Start_sym = nullptr;
bool Err = false;

int GetN(TreeNode_t **node)
{
    CHECK_ERROR;
    char *n_start_sym = Cur_sym;
    int val = 0;

    if (isalpha (*Cur_sym))
    {
        TreeData_t elem = {*Cur_sym++, PARAM};
        if (Cur_sym - n_start_sym == 0)
        {
            printf ("Syntax error: param expected, nothing found. (Position = %d)\n", Cur_sym - Start_sym);
            SET_ERROR;
            return 0;
        }
        *node = TreeNode_new (elem, nullptr, nullptr);
    }
    else
    {
        while (true)
        {
            if ('0' <= *Cur_sym && *Cur_sym <= '9')
            {
                val = val * 10 + *Cur_sym++ - '0';
            }
            else if (strchr("+-/*^()", *Cur_sym) || *Cur_sym == 0)
            {
                break;
            }
            else
            {
                printf ("Syntax error: unknown symbol \"%c\" (code = %d). (Position = %d)\n", *Cur_sym, *Cur_sym, Cur_sym - Start_sym);
                SET_ERROR;
                return 0;
            }
        }
        TreeData_t elem = {val, NUM};
        if (Cur_sym - n_start_sym == 0)
        {
            printf ("Syntax error: number expected, nothing found. (Position = %d)\n", Cur_sym - Start_sym);
            SET_ERROR;
            return 0;
        }
        *node = TreeNode_new (elem, nullptr, nullptr);
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

    if (*Cur_sym == '^')
    {
        Cur_sym++;

        GetX (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'^', OPER};
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

    if (isalpha (*Cur_sym) && isalnum (*(Cur_sym + 1)))
    {
        char func[MAX_FUNC_NAME_LENGTH] = "";
        int index = 0;
        while (isalnum (*Cur_sym))
        {
            func[index++] = *Cur_sym++;
        }
        if (*Cur_sym != '(') SET_ERROR;
        
        if (strcmp(func, "") == 0)
        {
            SET_ERROR;
        }
        #define FUNC(name, str, num)                            \
            else if (strcmp(func, str) == 0)                    \
            {                                                   \
                Cur_sym++; GetE(node);                          \
                CHECK_ERROR;                                    \
                TreeData_t elem = {num, FUNC};                  \
                *node = TreeNode_new (elem, nullptr, *node);    \
            }
        #include "funcs.h"
        #undef FUNC
        else
        {
            SET_ERROR;
        }

        if (*Cur_sym != ')')
        {
            SET_ERROR;
            printf ("Syntax error: \")\" expected, \"%c\" found. (Position = %d)\n", *Cur_sym, Start_sym - Cur_sym);
            return 0;
        }
        Cur_sym++;
    }
    else if (*Cur_sym == '(')
    {
        Cur_sym++;

        GetE (node);
        CHECK_ERROR;

        if (*Cur_sym != ')')
        {
            SET_ERROR;
            printf ("Syntax error: \")\" expected, \"%c\" found. (Position = %d)\n", *Cur_sym, Start_sym - Cur_sym);
            return 0;
        }
        Cur_sym++;
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

    if (*Cur_sym == '+')
    {
        Cur_sym++; 

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'+', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (*Cur_sym == '-')
    {
        Cur_sym++;

        GetE (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'-', OPER};
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

    if (*Cur_sym == '*')
    {
        Cur_sym++;

        GetT (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'*', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else if (*Cur_sym == '/')
    {
        Cur_sym++;

        GetT (&node2);
        CHECK_ERROR;

        TreeData_t elem = {'/', OPER};
        *node = TreeNode_new (elem, node1, node2);
    }
    else
    {
        *node = node1;
    }
    return 0;
}

int GetG0(char *expr, Tree_t *tree)
{
    RESET_ERROR;
    Cur_sym = expr;
    Start_sym = Cur_sym;
    GetE (&(tree->root));
    CHECK_ERROR;
    if (strlen (expr) != (Cur_sym - Start_sym)) SET_ERROR;
    return 0;
}

TreeNode_t *Diff(Tree_t *tree, TreeNode_t *node, char var, FILE *out_file)
{
    if (tree == nullptr || node == nullptr || !isalpha(var) || out_file == nullptr) return nullptr;

    elem_t add = {'+', OPER};
    elem_t sub = {'-', OPER};
    elem_t mul = {'*', OPER};
    elem_t div = {'/', OPER};
    elem_t pow = {'^', OPER};

    elem_t sin = {1, FUNC};
    elem_t cos = {2, FUNC};
    elem_t tg =  {3, FUNC};
    elem_t ln =  {4, FUNC};
    elem_t exp = {5, FUNC};

    elem_t minus_one = {-1, NUM};
    elem_t zero      = {0,  NUM};
    elem_t one       = {1,  NUM};
    elem_t two       = {2,  NUM};

    TreeNode_t *result = nullptr;

    #define ONE                 (TreeNode_new (one,       nullptr, nullptr))
    #define TWO                 (TreeNode_new (two,       nullptr, nullptr))
    #define MINUS_ONE           (TreeNode_new (minus_one, nullptr, nullptr))
    #define ZERO                (TreeNode_new (zero,      nullptr, nullptr))

    #define R                   (TreeNode_copy (node->right))
    #define L                   (TreeNode_copy (node->left))
    #define dR                  (Diff (tree, node->right, var, out_file))
    #define dL                  (Diff (tree, node->left,  var, out_file))
    #define COMPOSE(F1, F2)     (TreeNode_new (F1, nullptr, F2))

    #define MUL(f1, f2) (TreeNode_new (mul, f1, f2))
    #define DIV(f1, f2) (TreeNode_new (div, f1, f2))
    #define ADD(f1, f2) (TreeNode_new (add, f1, f2))
    #define SUB(f1, f2) (TreeNode_new (sub, f1, f2))
    #define POW(f1, f2) (TreeNode_new (pow, f1, f2))

    #define END_PRINT \
        fprintf (out_file, "\\Rightarrow ");\
        TreeNode_print_tex (result, out_file, tree, "(", ")", false);\
        fprintf (out_file, "\\\\\n");\
        return result;

    switch (node->data.type)
    {
    case FUNC:
        #define F R
        #define dF dR
        if (node->data.value == 1)
        {
            fprintf (out_file, "\\sin^{'}{x} = \\cos{x} : ");
            result =  MUL (COMPOSE (cos, F), dF);  
            END_PRINT;
        }
        else if (node->data.value == 2)
        {
            fprintf (out_file, "\\cos^{'}{x} = \\-sin{x} : ");
            result =  MUL (MUL (MINUS_ONE, COMPOSE (sin, F)), dF);  
            END_PRINT;
        }
        else if (node->data.value == 3)
        {
            fprintf (out_file, "\\tan^{'}{x} = 1/\\cos^{2}{x} : ");
            result = MUL (DIV (ONE, POW (COMPOSE (cos, F), TWO)), dF);
            END_PRINT;
        }
        else if (node->data.value == 4)
        {
            fprintf (out_file, "\\ln^{'}{x} = \\frac{1}{x} : ");
            result = MUL (DIV (ONE, F), dF);
            END_PRINT;
        }
        else if (node->data.value == 5)
        {
            fprintf (out_file, "\\exp{x})^{'} = \\exp{x} : ");
            result =  MUL (COMPOSE (exp, F), dF);
            END_PRINT;
        }
        else
        {
            return nullptr;
        }
        #undef F
        #undef dF
        break;
    case PARAM:
        if (node->data.value == var)
        {
            return ONE;
        }
        else
        {
            return ZERO;
        }
        break;
    case OPER:
        switch (node->data.value)
        {
        case '+':
            fprintf (out_file, "(f+g)^{'} = f^{'} + g^{'} : ");
            result =  ADD (dL, dR);
            END_PRINT;
            break;
        case '-':
            fprintf (out_file, "(f-g)^{'} = f^{'} - g^{'} : ");
            result =  SUB (dL, dR);
            END_PRINT;
            break;
        case '*':
            fprintf (out_file, "(f*g)^{'} = f^{'}g + g^{'}f : ");
            result =  ADD (MUL (dL, R), MUL (dR, L));
            END_PRINT;
            break;
        case '/':
            fprintf (out_file, "(f/g)^{'} = (f^{'}g - g^{'}f)/g^{2} : ");
            result =  DIV (SUB (MUL (dL, R), MUL (dR, L)), POW (R, TWO));
            END_PRINT;
            break;
        case '^':
            fprintf (out_file, "(f^{g})^{'} = f^{g-1}(gf^{'}+f\\ln{f}g^{'}) : ");
            result =  MUL (POW (L, SUB (R, ONE)), ADD ( MUL (dL, R), MUL (L, MUL (dR, COMPOSE (ln, L)))));
            END_PRINT;
            break;
        default:
            break;
        }
        break;
    case NUM:
        return ZERO;
        break;
    default:
        return nullptr;
        break;
    }
    return nullptr;
}

bool Fold_consts(TreeNode_t *node)
{
    if (node == nullptr) return false;
    if (node->data.type == OPER)
    {
        if ((node->left->data.type == NUM) && (node->right->data.type == NUM))
        {
            switch (node->data.value)
            {
            case '+':
                node->data.type = NUM;
                node->data.value = node->left->data.value + node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '-':
                node->data.type = NUM;
                node->data.value = node->left->data.value - node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '*':
                node->data.type = NUM;
                node->data.value = node->left->data.value * node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '/':
                return false;
                break;
            case '^':
                return false;
                break;
            default:
                return false;
                break;
            }
        }
        else
        {
            return Fold_consts(node->left) || Fold_consts(node->right);
        }
    }
    else
    {
        return Fold_consts(node->left) || Fold_consts(node->right);
    }
}

bool Delete_dead_nodes(TreeNode_t *node)
{
    if (node == nullptr) return false;
    if (node->data.type == OPER)
    {
        if ((node->left->data.type == NUM) && (node->left->data.value == 0))
        {
            if (node->data.value == '*' || node->data.value == '/')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.type = NUM;
                node->data.value = 0;
                return true;
            }
            else if (node->data.value == '+')
            {
                TreeNode_delete (node->left);
                TreeNode_t *dead_node = node->right;
                node->left = dead_node->left;
                node->right = dead_node->right;
                node->data.type = dead_node->data.type;
                node->data.value = dead_node->data.value;
                free (dead_node);
                dead_node = nullptr;
                return true;
            }
            else if (node->data.value == '-')
            {
                node->data.value = '*';
                node->left->data.value = -1;
                return true;
            }
            else
            {
                return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
            }
        }
        else if ((node->right->data.type == NUM) && (node->right->data.value == 0))
        {
            if (node->data.value == '*')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.type = NUM;
                node->data.value = 0;
                return true;
            }
            else if (node->data.value == '+' || node->data.value == '-')
            {
                TreeNode_delete (node->right);
                TreeNode_t *dead_node = node->left;
                node->left = dead_node->left;
                node->right = dead_node->right;
                node->data.type = dead_node->data.type;
                node->data.value = dead_node->data.value;
                free (dead_node);
                dead_node = nullptr;
                return true;
            }
            else
            {
                return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
            }
        }
        else if ((node->right->data.type == NUM) && (node->right->data.value == 1))
        {
            if (node->data.value == '*' || node->data.value == '/' || node->data.value == '^')
            {
                TreeNode_delete (node->right);
                TreeNode_t *dead_node = node->left;
                node->left = dead_node->left;
                node->right = dead_node->right;
                node->data.type = dead_node->data.type;
                node->data.value = dead_node->data.value;
                free (dead_node);
                dead_node = nullptr;
                return true;
            }
            else
            {
                return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
            }
        }
        else if ((node->left->data.type == NUM) && (node->left->data.value == 1))
        {
            if (node->data.value == '*')
            {
                TreeNode_delete (node->left);
                TreeNode_t *dead_node = node->right;
                node->left = dead_node->left;
                node->right = dead_node->right;
                node->data.type = dead_node->data.type;
                node->data.value = dead_node->data.value;
                free (dead_node);
                dead_node = nullptr;
                return true;
            }
            else
            {
                return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
            }
        }
        else if (TreeNode_compare(node->left, node->right))
        {
            elem_t two  = {2, NUM};
            if (node->data.value == '+')
            {
                node->data.value = '*';
                TreeNode_delete (node->right);
                node->right = TreeNode_new (two, nullptr, nullptr);
                return true;
            }
            else if (node->data.value == '*')
            {
                node->data.value = '^';
                TreeNode_delete (node->right);
                node->right = TreeNode_new (two, nullptr, nullptr);
                return true;
            }
            else if (node->data.value == '-')
            {
                TreeNode_delete (node->left);
                node->left = nullptr;
                TreeNode_delete (node->right);
                node->right = nullptr;
                node->data.type = NUM;
                node->data.value = 0;
                return true;
            }
            else if (node->data.value == '/')
            {
                TreeNode_delete (node->left);
                node->left = nullptr;
                TreeNode_delete (node->right);
                node->right = nullptr;
                node->data.type = NUM;
                node->data.value = 1;
                return true;
            }
        }
        else
        {
            return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
        }
    }
    else
    {
        return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
    }
}

bool Optimize(Tree_t *tree, FILE *out_file)
{
    if (tree == nullptr || out_file == nullptr) return true;
    bool result = false;
    do
    {
        result = Fold_consts (tree->root) || Delete_dead_nodes (tree->root);
        fprintf (out_file, "\t\t");
        Tree_print_tex (tree, out_file); 
        fprintf (out_file, "\\\\\n");
    }
    while (result);
    return true;
}