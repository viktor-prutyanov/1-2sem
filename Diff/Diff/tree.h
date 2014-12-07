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

enum type
{
    OPER = 0,
    NUM = 1,
    PARAM = 2,
    FUNC = 3
};

typedef struct elem_t
{
    int value;
    type type;
};

typedef elem_t TreeData_t;

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

/**
    @brief Recursively prints nodes to file

    @param node is first node to print
    @param file is output file

    @return true if success, otherwise false
*/
bool TreeNode_print_prefix(TreeNode_t *node, FILE *file);

/**
    @brief Recursively prints nodes to file

    @param node is first node to print
    @param file is output file

    @return true if success, otherwise false
*/
bool TreeNode_print_infix(TreeNode_t *node, FILE *file);

/**
    @brief Adds node to file

    @param new_left_node is new left node
    @param new_right_node is new right node
    @param tree is tree for adding
    @param node is existing (or null if there is no root) node for addition

    @return true if success, otherwise false
*/
bool TreeNode_add(Tree_t *tree, TreeNode_t *new_left_node, TreeNode_t *new_right_node, TreeNode_t *node);

/**
    @brief Creates new node

    @param left is new left node
    @param right is new right node
    @param data is data in new node

    @return new node
*/
TreeNode_t *TreeNode_new(TreeData_t data, TreeNode_t *left, TreeNode_t *right);

/**
    @brief Constructs tree

    @param tree is tree for construction

    @return true if success, otherwise false
*/
bool Tree_ctor(Tree_t *tree);

/**
    @brief Destructs tree

    @param tree is tree for destruction

    @return true if success, otherwise false
*/
bool Tree_dtor(Tree_t *tree);

/**
    @brief Recursively deletes nodes

    @param node is first node to delete

    @return true if success, otherwise false
*/
bool TreeNode_delete(TreeNode_t *node);

/**
    @brief Recursively prints tree to file

    @param tree is first node to print
    @param file is output file

    @return true if success, otherwise false
*/
bool Tree_print_infix(Tree_t *tree, FILE *file);

/**
    @brief Recursively prints tree to file

    @param tree is first node to print
    @param file is output file

    @return true if success, otherwise false
*/
bool Tree_print_prefix(Tree_t *tree, FILE *file);

/**
    @brief Recursively reads tree from file

    @param file is output file

    @return first readed node
*/
TreeNode_t *TreeNode_read(FILE *file);

/**
    @brief Recursively reads tree from file

    @param tree is tree to read
    @param file is output file

    @return true if success, otherwise false
*/
bool Tree_read(Tree_t *tree, FILE *file);

/**
    @brief Determines is node leaf or not?

    @param node is node to determine

    @return return 1 if leaf, 0 if not, -1 if error caused
*/
int TreeNode_is_leaf(TreeNode_t *node);

bool Tree_print_for_human(Tree_t *tree, FILE *file);

TreeNode_t *TreeNode_copy(TreeNode_t *node);

bool TreeNode_compare(TreeNode_t *node1, TreeNode_t *node2);

bool TreeNode_print_for_human(TreeNode_t *node, FILE *file, Tree_t *tree);

bool Tree_print_tex(Tree_t *tree, FILE *file);

bool TreeNode_print_tex(TreeNode_t *node, FILE *file, Tree_t *tree, const char *bracket1, const char *bracket2, bool is_by_func);