/**
*   Hash table
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "list.h"
#define PRIME_SIZE 101 //1009 32749

typedef char **HashTableItem_t;

typedef struct HashTable_t
{
    (List_t *) lists[PRIME_SIZE];
    unsigned int (*HashFunc)(HashTableItem_t item);
};

bool HashTable_ctor(HashTable_t *hashTable, unsigned int (*HashFunc)(HashTableItem_t item));

bool HashTable_dtor(HashTable_t *hashTable);

bool HashTable_dump(HashTable_t *hashTable);

bool HashTable_ok(HashTable_t *hashTable);

bool HashTable_add(HashTable_t *hashTable, HashTableItem_t item);