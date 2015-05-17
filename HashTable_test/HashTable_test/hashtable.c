/**
*   Hash table
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

bool HashTable_ctor(HashTable_t *hashTable, unsigned int (__fastcall *HashFunc)(HashTableItem_t item))
{
    if (hashTable == nullptr) return false;
    for (int i = 0; i < PRIME_SIZE; i++)
    {
        if (((hashTable->lists)[i] = (List_t *)calloc (1, sizeof(List_t))) == nullptr)
        {
            for (int j = 0; j < i; j++)
            {
                free ((hashTable->lists)[j]);
            }
            return false;
        }
        if (!List_ctor(hashTable->lists[i], 0))
        {
            for (int j = 0; j < i; j++)
            {
                free ((hashTable->lists)[j]);
            }
            return false;
        }
    }
    hashTable->HashFunc = HashFunc;
    return true;
}

bool HashTable_dtor(HashTable_t *hashTable)
{
    if (hashTable == nullptr) return false;
    for (int i = 0; i < PRIME_SIZE; i++)
    {
        if (!List_dtor (hashTable->lists[i])) return false;
        free (hashTable->lists[i]);
        hashTable->lists[i] = nullptr;
    }
    return true;
}

bool HashTable_ok(HashTable_t *hashTable)
{
    if (hashTable == nullptr) return false;
    for (int i = 0; i < PRIME_SIZE; i++)
    {
        if (!List_ok (hashTable->lists[i])) return false;
    }
    return true;
}

bool HashTable_dump(HashTable_t *hashTable)
{
    if (HashTable_ok (hashTable))
    {
        for (int i = 0; i < PRIME_SIZE; i++)
        {
            printf ("List #%d: ", i);
            List_dump (hashTable->lists[i]);
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool HashTable_add(HashTable_t *hashTable, HashTableItem_t item)
{
    //assert (HashTable_ok (hashTable));
    if (hashTable == nullptr) return false;

    ListNode_t *item_node = (ListNode_t *)calloc (1, sizeof(ListNode_t));
    item_node->data = item;
    int index = (hashTable->HashFunc (item)) % PRIME_SIZE;

    return List_insert_after (hashTable->lists[index], item_node, hashTable->lists[index]->tail); 
}

int HashTable_search(HashTable_t *hashTable, HashTableItem_t item, int (*compare )(const char *, const char *))
{
    //assert (HashTable_ok (hashTable));
    if (hashTable == nullptr) return false;

    int index = (hashTable->HashFunc (item)) % PRIME_SIZE;
    long long int num = hashTable->lists[index]->num;
    ListNode_t *cur_node = hashTable->lists[index]->head;

    for (int i = 0; i < num; i++)
    {
        if (compare (*item, *(cur_node->data)) == 0) 
        {
            return index;
        }
        cur_node = cur_node->next;
    }

    return -1;
}