/**
*   Doubly linked list
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include "list.h"
#include <stdlib.h>

int main()
{
    int num = 0;
    scanf ("%d", &num);

    List_t *list = (List_t *)calloc (1, sizeof(List_t));

    printf ("\nList construction:\n");
    List_ctor (list, num);
    List_dump (list);

    printf ("\nRemove head:\n");
    List_remove (list, list->head);
    List_dump (list);

    ListNode_t *node_to_insert1 = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    node_to_insert1->data = 5000;
    printf ("\nInsert (after) to middle:\n");
    List_insert_after (list, node_to_insert1, list->head->next);
    List_dump (list);

    ListNode_t *node_to_insert2 = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    node_to_insert2->data = 15000;
    printf ("\nInsert (after) to end:\n");
    List_insert_after (list, node_to_insert2, list->tail);
    List_dump (list);

    ListNode_t *node_to_insert3 = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    node_to_insert3->data = 20000;
    printf ("\nInsert (before) to middle:\n");
    List_insert_before (list, node_to_insert3, list->head->next->next);
    List_dump (list);

    ListNode_t *node_to_insert4 = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    node_to_insert4->data = 25000;
    printf ("\nInsert (before) to beginning:\n");
    List_insert_before (list, node_to_insert4, list->head);
    List_dump (list);

    printf ("\nRemove tail:\n");
    List_remove (list, list->tail);
    List_dump (list);

    printf ("\nRemove first element:\n");
    List_remove (list, list->head->next);
    List_dump (list);

    printf ("\nRemove not existing node:\n");
    List_remove (list, nullptr);
    List_dump (list);

    printf ("\nList destruction:\n");
    List_dtor (list);
    List_dump (list);
    free (list);
    list = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif

    return 0;
}