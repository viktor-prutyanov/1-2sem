/**
*   Assembly code generator
*
*   @date 12.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include "synt.h"

bool Generate (TreeNode_t *node, FILE *file);
bool Calculate (TreeNode_t *node, FILE *file);