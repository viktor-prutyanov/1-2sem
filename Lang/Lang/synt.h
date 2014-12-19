/**
*   Syntax analyzer
*
*   @date 12.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "tree.h"

int GetG0(Token_t *tokens, Tree_t *tree, int tokens_amount);

int GetE(TreeNode_t **node);
int GetT(TreeNode_t **node);
int GetX(TreeNode_t **node);
int GetP(TreeNode_t **node);
int GetN(TreeNode_t **node);
int GetS(TreeNode_t **node);
int GetB(TreeNode_t **node);
int GetQ(TreeNode_t **node);
int GetF(TreeNode_t **node);
int GetBE(TreeNode_t **node);
int GetW(TreeNode_t **node);