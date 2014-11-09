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

int HashFunc(HashTableItem_t item);

int main()
{
    HashTable_t *hashTable = (HashTable_t *)calloc (1, sizeof(HashTable_t)); 
    HashTable_ctor (hashTable, HashFunc);
    HashTable_dump (hashTable);

    char **item = (char **)calloc (1, sizeof (char *));
    char str[] = "Example string!";
    *item = str;
    HashTable_add (hashTable, item);

    HashTable_dump (hashTable);
    HashTable_dtor (hashTable);
    free (hashTable);
    hashTable = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif
    return 0;
}

int HashFunc(HashTableItem_t item)
{
    return 5;
}