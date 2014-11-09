/**
*   Doubly linked list
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

bool List_ctor(List_t *list, int num)
{
    if (list == nullptr ||  num < 1) return false;

    list->num = num;

    list->head = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    if (list->head == nullptr) return false;
    list->head->data = 0;
    list->head->prev = nullptr;

    ListNode_t *prev_node, *cur_node;
    prev_node = list->head;
    cur_node = list->head;

    for (int i = 1; i < num; i++)
    {
        cur_node = (ListNode_t *)calloc (1, sizeof(ListNode_t));
        if (cur_node == nullptr) return false;
        cur_node->data = i;
        cur_node->prev = prev_node;
        prev_node->next = cur_node;
        prev_node = cur_node;
    }

    list->tail = cur_node;
    list->tail->next = nullptr;

    return true;
}

bool List_ok(List_t *list)
{
    if (list == nullptr || list->head == nullptr || list->tail == nullptr || list->num < 1) return false;
    ListNode_t *cur_node = list->head;
    if (list->head->prev != nullptr && list->tail->next != nullptr) return false;
    for (int i = 0; i < list->num; i++)
    {
        if (cur_node != list->tail && cur_node != cur_node->next->prev) return false;
        cur_node = cur_node->next;
    }
    return true;
}

bool List_dump(List_t *list)
{
    if (List_ok (list))
    {
        printf ("List [0x%p] is OK.\n", list);
        ListNode_t *cur_node = list->head;
        for (int i = 0; i < list->num; i++)
        {
            printf ("\t[0x%p] %6d prev:0x%p next:0x%p\n", cur_node, cur_node->data, cur_node->prev, cur_node->next);
            cur_node = cur_node->next;
        }
        return true;
    }
    else
    {
        printf ("List [0x%p] is NOT OK.\n", list);
        return false;
    }
}

bool List_remove(List_t *list, ListNode_t *node)
{
    assert (List_ok (list));
    if (list == nullptr || node == nullptr || list->num < 2) return false;
    if (node == list->head)
    {
        list->head = list->head->next;
        list->head->prev = nullptr;
    }
    else if (node == list->tail)
    {
        list->tail = list->tail->prev;
        list->tail->next = nullptr;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    (list->num)--;
    free (node);
    node = nullptr;
    return true;
}

bool List_dtor(List_t *list)
{
    assert (List_ok (list));
    if (list == nullptr) return false;

    ListNode_t *next_node, *cur_node;
    cur_node = list->head;

    for (int i = 0; i < list->num; i++)
    {
        next_node = cur_node->next;
        cur_node->data = -1;
        cur_node->next = nullptr;
        cur_node->prev = nullptr;
        free (cur_node);
        cur_node = next_node;
    }
    
    list->num = 0;
    list->head = nullptr;
    list->tail = nullptr;

    return true;
}

bool List_insert_after(List_t *list, ListNode_t *node_to_insert, ListNode_t *node)
{
    assert (List_ok (list));
    if (list == nullptr || node == nullptr || node_to_insert == nullptr) return false;

    node_to_insert->prev = node;
    node_to_insert->next = node->next;
    if (node == list->tail)
    {
        list->tail = node_to_insert; 
    }
    else
    {
        node->next->prev = node_to_insert;
    }

    node->next = node_to_insert;

    (list->num)++;

    return true;
}

bool List_insert_before(List_t *list, ListNode_t *node_to_insert, ListNode_t *node)
{
    assert (List_ok (list));
    if (list == nullptr || node == nullptr || node_to_insert == nullptr) return false;

    node_to_insert->next = node;
    node_to_insert->prev = node->prev;
    if (node == list->head)
    {
        list->head = node_to_insert; 
    }
    else
    {
        node->prev->next = node_to_insert;
    }

    node->prev = node_to_insert;

    (list->num)++;

    return true;
}