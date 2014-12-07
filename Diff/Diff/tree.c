/**
*   Binary tree
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "diff.h"
#include <stdlib.h>
#include <stdio.h>

bool Tree_ctor(Tree_t *tree)
{
    if (tree == nullptr) return false;
    tree->root = nullptr;

    return true;
}

TreeNode_t *TreeNode_new(TreeData_t data, TreeNode_t *left, TreeNode_t *right)
{
    TreeNode_t *node = (TreeNode_t *)calloc (1, sizeof (TreeNode_t));
    if (node == nullptr) return nullptr;

    node->data = data;
    node->left = left;
    node->right = right;

    return node;
}

bool TreeNode_add(Tree_t *tree, TreeNode_t *new_left_node, TreeNode_t *new_right_node, TreeNode_t *node)
{
    if (tree == nullptr) return false;
    if (new_left_node == nullptr && new_right_node == nullptr) return false;

    if (node == nullptr)
    {
        if (tree->root == nullptr)
        {
            if (new_left_node == nullptr)
            {
                tree->root = new_right_node;
            }
            else
            {
                tree->root = new_left_node;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        node->left = new_left_node;
        node->right = new_right_node; 
    }

    return true;
}

bool TreeNode_print_prefix(TreeNode_t *node, FILE *file)
{
    if (node == nullptr || file == nullptr) return false;
    if (node->data.type == OPER || node->data.type == PARAM)
    {
        fprintf (file, "(%c" , node->data.value);
    }
    else if (node->data.type == NUM)
    {
        fprintf (file, "(%d" , node->data.value);
    }
    else if (node->data.type == FUNC)
    {
        #define FUNC(name, str, num)                                    \
            if (num == (node->data.value)) fprintf (file, "(%s" , str);
        #include "funcs.h"
        #undef FUNC
    }
    if (node->left != nullptr) TreeNode_print_prefix (node->left, file);
    if (node->right != nullptr) TreeNode_print_prefix (node->right, file);
    fprintf (file, ")");
    return true;
}


bool TreeNode_print_infix(TreeNode_t *node, FILE *file)
{
    if (node == nullptr || file == nullptr) return false;
    fprintf (file, "(");
    if (node->left != nullptr) TreeNode_print_infix (node->left, file);
    if (node->data.type == OPER || node->data.type == PARAM)
    {
        fprintf (file, "%c" , node->data.value);
    }
    else if (node->data.type == NUM)
    {
        fprintf (file, "%d" , node->data.value);
    }
    else if (node->data.type == FUNC)
    {
        #define FUNC(name, str, num)                                    \
            if (num == (node->data.value)) fprintf (file, "%s" , str);
        #include "funcs.h"
        #undef FUNC
    }
    if (node->right != nullptr) TreeNode_print_infix (node->right, file);
    fprintf (file, ")");
    return true;
}

bool TreeNode_print_for_human(TreeNode_t *node, FILE *file, Tree_t *tree)
{
    if (node == nullptr || file == nullptr) return false;
    if (!TreeNode_is_leaf (node) && node != tree->root)
    {
        fprintf (file, "(");
    }
    if (node->left != nullptr) TreeNode_print_for_human (node->left, file, tree);
    if (node->data.type == OPER || node->data.type == PARAM)
    {
        fprintf (file, "%c" , node->data.value);
    }
    else if (node->data.type == NUM)
    {
        fprintf (file, "%d" , node->data.value);
    }
    else if (node->data.type == FUNC)
    {
        #define FUNC(name, str, num)                                    \
            if (num == (node->data.value)) fprintf (file, "%s" , str);
        #include "funcs.h"
        #undef FUNC
    }
    if (node->right != nullptr) TreeNode_print_for_human (node->right, file, tree);
    if (!TreeNode_is_leaf (node) && node != tree->root)
    {
        fprintf (file, ")");
    }
    return true;
}

bool TreeNode_print_tex(TreeNode_t *node, FILE *file, Tree_t *tree, const char *bracket1, const char *bracket2, bool is_by_func)
{
    if (node == nullptr || file == nullptr) return false;
    if (!TreeNode_is_leaf (node) && node != tree->root || is_by_func)
    {
        fprintf (file, bracket1);
    }
    if (node->left != nullptr) TreeNode_print_tex (node->left, file, tree, "(", ")", false);
    if (node->data.type == OPER || node->data.type == PARAM)
    {
        fprintf (file, "%c" , node->data.value);
    }
    else if (node->data.type == NUM)
    {
        fprintf (file, "%d" , node->data.value);
    }
    else if (node->data.type == FUNC)
    {
        #define FUNC(name, str, num)                                    \
            if (num == (node->data.value)) fprintf (file, "\\%s" , str);
        #include "funcs.h"
        #undef FUNC
    }
    if (node->right != nullptr)
    {
        if (node->data.type == FUNC)
        {
            TreeNode_print_tex (node->right, file, tree, "{(", ")}", true);
        }
        else if (node->data.type == OPER && node->data.value == '^' )
        {
            TreeNode_print_tex (node->right, file, tree, "{", "}", false);
        }
        else
        {
            TreeNode_print_tex (node->right, file, tree, "(", ")", false);
        }
    }
    if (!TreeNode_is_leaf (node) && node != tree->root || is_by_func)
    {
        fprintf (file, bracket2);
    }
    return true;
}

bool TreeNode_delete(TreeNode_t *node)
{
    if (node == nullptr) return false;
    if (node->left != nullptr)
    {
        TreeNode_delete (node->left);
        free (node->left);
        node->left = nullptr;
    }
    if (node->right != nullptr)
    {
        TreeNode_delete (node->right);
        free (node->right);
        node->right = nullptr;
    }
    return true;
}

bool Tree_dtor(Tree_t *tree)
{
    if (tree == nullptr) return false;
    TreeNode_delete (tree->root);
    free (tree->root);
    tree->root = nullptr;
    return true;
}

bool Tree_print_prefix(Tree_t *tree, FILE *file)
{
    if (tree == nullptr || file == nullptr) return false;
    return TreeNode_print_prefix (tree->root, file);
}

bool Tree_print_infix(Tree_t *tree, FILE *file)
{
    if (tree == nullptr || file == nullptr) return false;
    return TreeNode_print_infix (tree->root, file);
}

bool Tree_print_for_human(Tree_t *tree, FILE *file)
{
    if (tree == nullptr || file == nullptr) return false;
    return TreeNode_print_for_human (tree->root, file, tree);
}

bool Tree_print_tex(Tree_t *tree, FILE *file)
{
    if (tree == nullptr || file == nullptr) return false;
    bool result = TreeNode_print_tex (tree->root, file, tree, "(", ")", false);
    return result;
}

TreeNode_t *TreeNode_read(FILE *file)
{
    if (file == nullptr) return false;
    TreeData_t data;
    if (fscanf (file, "(%d" , &data) > 0)
    {
        TreeNode_t *left = TreeNode_read (file);
        TreeNode_t *right = TreeNode_read (file);
        TreeNode_t *node = TreeNode_new (data, left, right);
        fscanf (file, ")");
        return node;
    }
    else
    {
        return nullptr;
    }
}

bool Tree_read(Tree_t *tree, FILE *file)
{
    if (file == nullptr || tree == nullptr) return false;
    TreeNode_add(tree, TreeNode_read (file), nullptr, nullptr);
    return true;
}

int TreeNode_is_leaf(TreeNode_t *node)
{
    if (node == nullptr) return -1;
    if (node->left == nullptr && node->right == nullptr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

TreeNode_t *TreeNode_copy(TreeNode_t *node)
{
    if (node == nullptr) return nullptr;
    return TreeNode_new (node->data, TreeNode_copy (node->left), TreeNode_copy (node->right));
}

bool TreeNode_compare(TreeNode_t *node1, TreeNode_t *node2)
{
    if (node1 == nullptr || node2 == nullptr) return false;
    if (node1->data.type == node2->data.type && node1->data.value == node2->data.value)
    {
        if (TreeNode_is_leaf (node1) && TreeNode_is_leaf (node2))
        {
            return true;
        }
        else
        {
            return TreeNode_compare (node1->left, node2->left) && TreeNode_compare (node1->right, node2->right);
        }
    }
    else
    {
        return false;
    }
}