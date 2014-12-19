/**
*   Assembly code generator
*
*   @date 12.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "gen.h"
#include <stdlib.h>
#include <ctype.h>

#define MIN_DIFF_NUM 11
#define MAX_DIFF_NUM 14

TreeNode_t *Diff(TreeNode_t *node, char var);
bool Optimize(TreeNode_t *node);
bool Delete_dead_nodes(TreeNode_t *node);
bool Fold_consts(TreeNode_t *node);
unsigned int pow(unsigned int base, unsigned int power);

int Labels_amount = 1;

bool Generate (TreeNode_t *node, FILE *file)
{
    if (node == nullptr || node->data.type != CONTROL || node->data.value != '~' || file == nullptr) return false;

    if (node->left->data.type == L_OPERATOR && node->left->data.value == '=')
    {
        Calculate (node->left->right, file);
        fprintf (file, "pop %cx\n", node->left->left->data.value);  
    }
    else if (node->left->data.type == FUNCTION)
    {
        if (false) {}
        #define FUNC(name, str, num, is_math, is_calculatable, code)                               \
        else if (num == node->left->data.value && !is_math)                                        \
        {                                                                                          \
            if (is_calculatable)                                                                   \
            {                                                                                      \
                Calculate (node->left->right, file);                                               \
                fprintf (file, code, node->left->right->data.value, node->left->right->data.value);\
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                fprintf (file, code, node->left->right->data.value, node->left->right->data.value);\
            }                                                                                      \
        }              
        #include "funcs.h"
        #undef FUNC
        else
        {
            return false;
        }
    }
    else if (node->left->data.type == W_OPERATOR)
    {
        if (node->left->data.value == 1)
        {
            Calculate (node->left->left->right, file);
            Calculate (node->left->left->left, file);
            if (false)
            {

            }
            #define L_OPER(name, str, num, code, invert_code)   \
            else if (num == node->left->left->data.value)       \
            {                                                   \
                fprintf (file, invert_code);                    \
            }
            #include "l_opers.h"
            #undef L_OPER
            else if (node->left->left->data.value == '<')
            {
                fprintf (file, "jae");                    
                
            }
            else if (node->left->left->data.value == '>')
            {
                fprintf (file, "jbe");                    
            }
            else
            {
                return false;
            }
            fprintf (file, " _l%d\n", Labels_amount);
            Generate (node->left->right, file);
            fprintf (file, ":l%d\n", Labels_amount);
            Labels_amount++;
        }
        else if (node->left->data.value == 2)
        {
            fprintf (file, ":l%d\n", Labels_amount);
            Calculate (node->left->left->right, file);
            Calculate (node->left->left->left, file);
            if (false)
            {

            }
            #define L_OPER(name, str, num, code, invert_code)   \
            else if (num == node->left->left->data.value)       \
            {                                                   \
                fprintf (file, invert_code);                    \
            }
            #include "l_opers.h"
            #undef L_OPER
            else if (node->left->left->data.value == '<')
            {
                fprintf (file, "jae");                    
                
            }
            else if (node->left->left->data.value == '>')
            {
                fprintf (file, "jbe");                    
            }
            else
            {
                return false;
            }
            fprintf (file, " _l%d\n", Labels_amount + 1);
            Generate (node->left->right, file);
            fprintf (file, "jmp _l%d\n", Labels_amount);
            fprintf (file, ":l%d\n", Labels_amount + 1);
            Labels_amount += 2;
        }
        else 
        {
            return false;
        }
    }

    if (node->right == nullptr)
    {
        return true;
    }
    else if (node->right->data.type == CONTROL && node->right->data.value == '~')
    {
        return Generate (node->right, file);
    }
    else
    {
        return true;
    }

    return true;
}

bool Calculate (TreeNode_t *node, FILE *file)
{
    if (file == nullptr) return false;
    if (node == nullptr) return true;
 
    if (node->data.type == FUNCTION && (MIN_DIFF_NUM <= node->data.value && node->data.value <= MAX_DIFF_NUM))
    {
        TreeNode_t *diff_root = Diff (node->right, node->data.value - MIN_DIFF_NUM + 'a');
        Optimize (diff_root);
        bool result = Calculate (diff_root, file);
        TreeNode_delete (diff_root);
        return result;
    }

    Calculate (node->right, file);
    Calculate (node->left, file);

    if (node->data.type == NUMBER)
    {
        fprintf (file, "push %d\n", node->data.value);
        return true;
    }
    else if (node->data.type == VARIABLE)
    {
        fprintf (file, "push %cx\n", node->data.value);
        return true;
    }
    else if (node->data.type == A_OPERATOR)
    {
        switch (node->data.value)
        {
        case '+':
            fprintf (file, "add\n");
            break;
        case '-':
            fprintf (file, "sub\n");
            break;
        case '*':
            fprintf (file, "mul\n");
            break;
        case '/':
            fprintf (file, "div\n");
            break;
        case '^':
            fprintf (file, "pow\n");
            break;
        default:
            break;
        }
        return true;
    }
    else if (node->data.type == FUNCTION)
    {
        if (false) {}
        #define FUNC(name, str, num, is_math) else if (node->data.value == num && is_math) { fprintf (file, str); fprintf (file, "\n"); return true;}
        #include "funcs.h"
        #undef FUNC
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

TreeNode_t *Diff(TreeNode_t *node, char var)
{
    if (node == nullptr || !isalpha(var)) return nullptr;

    TreeData_t add = {'+', A_OPERATOR}; 
    TreeData_t sub = {'-', A_OPERATOR};
    TreeData_t mul = {'*', A_OPERATOR};
    TreeData_t div = {'/', A_OPERATOR};
    TreeData_t pow = {'^', A_OPERATOR};
    
    TreeData_t sin = {1, FUNCTION};
    TreeData_t cos = {2, FUNCTION};
    TreeData_t tan = {3, FUNCTION};
    TreeData_t exp = {4, FUNCTION};
    TreeData_t ln  = {5, FUNCTION};
    
    TreeData_t minus_one = {-1, NUMBER};
    TreeData_t zero      = {0,  NUMBER};
    TreeData_t one       = {1,  NUMBER};
    TreeData_t two       = {2,  NUMBER};

    TreeNode_t *result = nullptr;

    #define ONE                 (TreeNode_new (one,       nullptr, nullptr))
    #define TWO                 (TreeNode_new (two,       nullptr, nullptr))
    #define MINUS_ONE           (TreeNode_new (minus_one, nullptr, nullptr))
    #define ZERO                (TreeNode_new (zero,      nullptr, nullptr))

    #define R                   (TreeNode_copy (node->right))
    #define L                   (TreeNode_copy (node->left))
    #define dR                  (Diff (node->right, var))
    #define dL                  (Diff (node->left,  var))
    #define COMPOSE(F1, F2)     (TreeNode_new (F1, nullptr, F2))

    #define MUL(f1, f2) (TreeNode_new (mul, f1, f2))
    #define DIV(f1, f2) (TreeNode_new (div, f1, f2))
    #define ADD(f1, f2) (TreeNode_new (add, f1, f2))
    #define SUB(f1, f2) (TreeNode_new (sub, f1, f2))
    #define POW(f1, f2) (TreeNode_new (pow, f1, f2))

    switch (node->data.type)
    {
    case FUNCTION:
        #define F R
        #define dF dR
        if (node->data.value == 1)
        {
            return  MUL (COMPOSE (cos, F), dF);  
        }
        else if (node->data.value == 2)
        {
            return MUL (MUL (MINUS_ONE, COMPOSE (sin, F)), dF);  
        }
        else if (node->data.value == 3)
        {
            return MUL (DIV (ONE, POW (COMPOSE (cos, F), TWO)), dF);
        }
        else if (node->data.value == 4)
        {
            return MUL (DIV (ONE, F), dF);
        }
        else if (node->data.value == 5)
        {
            return MUL (COMPOSE (exp, F), dF);
        }
        else
        {
            return nullptr;
        }
        #undef F
        #undef dF
        break;
    case VARIABLE:
        if (node->data.value == var)
        {
            return ONE;
        }
        else
        {
            return ZERO;
        }
        break;
    case A_OPERATOR:
        switch (node->data.value)
        {
        case '+':
            return ADD (dL, dR);
            break;
        case '-':
            return SUB (dL, dR);
            break;
        case '*':
            return ADD (MUL (dL, R), MUL (dR, L));
            break;
        case '/':
            return DIV (SUB (MUL (dL, R), MUL (dR, L)), POW (R, TWO));
            break;
        case '^':
            return MUL (POW (L, SUB (R, ONE)), ADD ( MUL (dL, R), MUL (L, MUL (dR, COMPOSE (ln, L)))));
            break;
        default:
            break;
        }
        break;
    case NUMBER:
        return ZERO;
        break;
    default:
        return nullptr;
        break;
    }
    return nullptr;

    #undef ONE        
    #undef TWO        
    #undef MINUS_ONE  
    #undef ZERO       
    #undef R         
    #undef L         
    #undef dR    
    #undef dL    
    #undef COMPOSE
    #undef MUL
    #undef DIV
    #undef ADD
    #undef SUB
    #undef POW
}

bool Fold_consts(TreeNode_t *node)
{
    if (node == nullptr) return false;
    if (node->data.type == A_OPERATOR)
    {
        if ((node->left->data.type == NUMBER) && (node->right->data.type == NUMBER))
        {
            switch (node->data.value)
            {
            case '+':
                node->data.type = NUMBER;
                node->data.value = node->left->data.value + node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '-':
                node->data.type = NUMBER;
                node->data.value = node->left->data.value - node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '*':
                node->data.type = NUMBER;
                node->data.value = node->left->data.value * node->right->data.value;
                TreeNode_delete (node->left);
                TreeNode_delete (node->right);
                node->left = nullptr;
                node->right = nullptr;
                return true;
                break;
            case '^':
                if (node->left->data.value > 0 && node->right->data.value > 0)
                {
                    node->data.type = NUMBER;
                    node->data.value = pow (node->left->data.value, node->right->data.value);
                    TreeNode_delete (node->left);
                    TreeNode_delete (node->right);
                    node->left = nullptr;
                    node->right = nullptr;
                    return true;
                }
                else
                {
                    return false;
                }
                break;
            case '/':
                if (node->left->data.value > node->right->data.value)
                {
                    for (int i = 2; i < node->left->data.value; i++)
                    {
                        if (node->left->data.value % i == 0 && node->right->data.value % i == 0)
                        {
                            node->left->data.value  /= i;
                            node->right->data.value /= i;
                            return true;
                        }
                    }
                    return false;
                }
                else
                {
                    for (int i = 2; i < node->right->data.value; i++)
                    {
                        if (node->left->data.value % i == 0 && node->right->data.value % i == 0)
                        {
                            node->left->data.value  /= i;
                            node->right->data.value /= i;
                            return true;
                        }
                    }
                    return false;
                }
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
    if (node->data.type == A_OPERATOR)
    {
        if ((node->data.value == '*') && (node->right->data.value == '/') && (node->right->data.type == A_OPERATOR) 
            && (node->right->left->data.value == 1) && (node->right->left->data.type == NUMBER))
        {
            node->data.value = '/';
            TreeNode_t *dead_node = node->right;
            node->right = node->right->right;
            dead_node->right = nullptr;
            TreeNode_delete (dead_node);
            free (dead_node);
            dead_node = nullptr;
            
            return true;
        }
        else if ((node->data.value == '*') && (node->left->data.value == '/') && (node->left->data.type == A_OPERATOR)
            && (node->left->left->data.value == 1) && (node->left->left->data.type == NUMBER))
        {

            node->data.value = '/';
            TreeNode_t *dead_node = node->left;
            node->left = node->right;
            node->right = dead_node->right;
            dead_node->right = nullptr;
            TreeNode_delete (dead_node);
            free (dead_node);
            dead_node = nullptr;

            return true;
        }
        else if ((node->left->data.type == NUMBER) && (node->left->data.value == 0))
        {
            if (node->data.value == '*' || node->data.value == '/' || node->data.value == '^')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.type = NUMBER;
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
        else if ((node->right->data.type == NUMBER) && (node->right->data.value == 0))
        {
            if (node->data.value == '*')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.type = NUMBER;
                node->data.value = 0;
                return true;
            }
            else if (node->data.value == '^')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.type = NUMBER;
                node->data.value = 1;
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
        else if ((node->right->data.type == NUMBER) && (node->right->data.value == 1))
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
        else if ((node->left->data.type == NUMBER) && (node->left->data.value == 1))
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
            else if (node->data.value == '^')
            {
                TreeNode_delete (node->right);
                TreeNode_delete (node->left);
                node->left = nullptr;
                node->right = nullptr;
                node->data.value = 1;
                node->data.type = NUMBER;
                return true;
            }
            else
            {
                return Delete_dead_nodes(node->left) || Delete_dead_nodes(node->right);
            }
        }
        else if (TreeNode_compare(node->left, node->right))
        {
            TreeData_t two  = {2, NUMBER};
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
                node->data.type = NUMBER;
                node->data.value = 0;
                return true;
            }
            else if (node->data.value == '/')
            {
                TreeNode_delete (node->left);
                node->left = nullptr;
                TreeNode_delete (node->right);
                node->right = nullptr;
                node->data.type = NUMBER;
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
    return false;
}

bool Optimize(TreeNode_t *node)
{
    if (node == nullptr) return true;
    bool res1 = false, res2 = false;
    do
    {
        res1 = Fold_consts (node); 
        res2 = Delete_dead_nodes (node);
    }
    while (res1 || res2);
    return true;
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