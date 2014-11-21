/**
*   Binary tree
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>

typedef int TreeData_t;

typedef struct TreeNode_t
{
    TreeData_t data;
    TreeNode_t *left;
    TreeNode_t *right;
};

typedef struct Tree_t
{
    TreeNode_t *root;
};

bool TreeNode_print(TreeNode_t *node, FILE *file);
bool TreeNode_add(Tree_t *tree, TreeNode_t *new_left_node, TreeNode_t *new_right_node, TreeNode_t *node);
TreeNode_t *TreeNode_new(TreeData_t data, TreeNode_t *left, TreeNode_t *right);
bool Tree_ctor(Tree_t *tree);
bool Tree_dtor(Tree_t *tree);
bool TreeNode_delete(TreeNode_t *node);
bool Tree_print(Tree_t *tree, FILE *file);
TreeNode_t *TreeNode_read(FILE *file);
bool Tree_read(Tree_t *tree, FILE *file);
int TreeNode_is_leaf(TreeNode_t *node);