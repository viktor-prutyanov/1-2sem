/**
*   Doubly linked list
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

typedef char **ListData_t;

typedef struct ListNode_t
{
    ListData_t data;
    ListNode_t *next;
    ListNode_t *prev;
};

typedef struct List_t
{
    long long int num;
    ListNode_t *head;
    ListNode_t *tail;
};

/**
    @brief List constructor

    @param list is List_t list for construction
    @param num is int number of elements

    @return true if construction successful, else returns false
*/
bool List_ctor(List_t *list, long long int num);

/**
    @brief List verification

    @param list is List_t list for verification

    @return true if verification successful, else returns false
*/
bool List_ok(List_t *list);

/**
    @brief Creates list dump

    @param list is List_t list for dump creation

    @return true if dumping successful, else returns false
*/
bool List_dump(List_t *list);

/**
    @brief Removes node

    @param list is List_t list for removing 
    @param node is ListNode_t node for removing

    @return true if removing successful, else returns false
*/
bool List_remove(List_t *list, ListNode_t *node);

/**
    @brief Destruction of list

    @param list is List_t list for node removing
    @param node is ListNode_t node for removing

    @return true if removing successful, else returns false
*/
bool List_dtor(List_t *list);

/**
    @brief Insertion node to list (before)

    @param list is List_t list for insertion
    @param node is ListNode_t node for insertion
    @param node_to_insert is ListNode_t node what will be insertion before

    @return true if insertion successful, else returns false
*/
bool List_insert_before(List_t *list, ListNode_t *node_to_insert, ListNode_t *node);

/**
    @brief Insertion node to list (after)

    @param list is List_t list for insertion
    @param node is ListNode_t node for insertion
    @param node_to_insert is ListNode_t node what will be insertion after

    @return true if insertion successful, else returns false
*/
bool List_insert_after(List_t *list, ListNode_t *node_to_insert, ListNode_t *node);
