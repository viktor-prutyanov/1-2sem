/**
*   Expression calculator
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "tree.h"

int GetG0(char *expr, Tree_t *tree);

int GetE(TreeNode_t **node);
int GetT(TreeNode_t **node);
int GetX(TreeNode_t **node);
int GetP(TreeNode_t **node);
int GetN(TreeNode_t **node);

TreeNode_t *Diff(Tree_t *tree, TreeNode_t *node, char var, FILE *out_file);
bool Fold_consts(TreeNode_t *node);
bool Delete_dead_nodes(TreeNode_t *node);
bool Optimize(Tree_t *tree, FILE *out_file);