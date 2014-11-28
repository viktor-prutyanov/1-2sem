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
#define PRIME_SIZE 1009 //32749

typedef char **HashTableItem_t;

typedef struct HashTable_t
{
    (List_t *) lists[PRIME_SIZE];
    unsigned int (*HashFunc)(HashTableItem_t item);
};

/**
    @brief Constructs the hash table

    @param hashTable is HashTable_t table to construct
    @param HashFunc is hash function.

    @return true if construction successful, else returns false
*/
bool HashTable_ctor(HashTable_t *hashTable, unsigned int (*HashFunc)(HashTableItem_t item));

/**
    @brief Destructs the hash table

    @param hashTable is HashTable_t table to destruct

    @return true if destruction successful, else returns false
*/
bool HashTable_dtor(HashTable_t *hashTable);

/**
    @brief Prints dump of the hash table

    @param hashTable is HashTable_t table to dump

    @return true if dumping successful, else returns false
*/
bool HashTable_dump(HashTable_t *hashTable);

/**
    @brief Verifies the hash table  

    @param hashTable is HashTable_t table to verify

    @return true if verification successful, else returns false
*/
bool HashTable_ok(HashTable_t *hashTable);

/**
    @brief Adds the item to the hash table.

    @param hashTable is HashTable_t table to addition.
    @param item is HashTableItem_t item to add.
    
    @return true if addition successful, else returns false
*/
bool HashTable_add(HashTable_t *hashTable, HashTableItem_t item);

/**
    @brief Search of item

    @param hashTable is table for search
    @param item is item to search
    @param comapare is function of comparsion of items

    @return index
*/
int HashTable_search(HashTable_t *hashTable, HashTableItem_t item, int (*compare )(const char *, const char *));