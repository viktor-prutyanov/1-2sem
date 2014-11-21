/**
*   Binary tree
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "tree.h"
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

bool TreeNode_print(TreeNode_t *node, FILE *file)
{
    if (node == nullptr || file == nullptr) return false;
    fprintf (file, "(%d" , node->data);
    if (node->left != nullptr) TreeNode_print (node->left, file);
    if (node->right != nullptr) TreeNode_print (node->right, file);
    fprintf (file, ")");
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

bool Tree_print(Tree_t *tree, FILE *file)
{
    if (tree == nullptr || file == nullptr) return false;
    return TreeNode_print (tree->root, file);
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