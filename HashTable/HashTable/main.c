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
#include <ctype.h>
#include <locale.h>
#include <string.h>

#define TEST(number)                                                                   \
    printf ("Test ");                                                                  \
    printf (#number);                                                                  \
    printf (" started.\n");                                                            \
    run_test (out_file##number, words_amount, current_locale, HashFunc##number, words);\

#define MAX_WORD_SIZE 64

unsigned int get_ru_code (char c);
int get_next_word (FILE *in_file, char* word, _locale_t locale);

void run_test (FILE *out_file, long long int words_amount, _locale_t current_locale, unsigned int (*HashFunc)(HashTableItem_t item), char **words);
bool hashtable_nums_to_csv (HashTable_t *hashTable, FILE *out_file);

unsigned int HashFunc1 (HashTableItem_t item);
unsigned int HashFunc2 (HashTableItem_t item);
unsigned int HashFunc3 (HashTableItem_t item);
unsigned int HashFunc4 (HashTableItem_t item);
unsigned int HashFunc5 (HashTableItem_t item);
unsigned int HashFunc6 (HashTableItem_t item);

int main()
{
    
    setlocale(LC_ALL, "Russian");
    _locale_t current_locale = _get_current_locale();
    
    FILE *in_file = fopen ("..\\1_.txt", "r");

    FILE *out_file1 = fopen ("..\\1.csv", "w");
    FILE *out_file2 = fopen ("..\\2.csv", "w");
    FILE *out_file3 = fopen ("..\\3.csv", "w");
    FILE *out_file4 = fopen ("..\\4.csv", "w");
    FILE *out_file5 = fopen ("..\\5.csv", "w");
    FILE *out_file6 = fopen ("..\\6.csv", "w");

    if (in_file == nullptr || out_file1 == nullptr || out_file2 == nullptr || out_file3 == nullptr || out_file4 == nullptr || out_file5 == nullptr || out_file6 == nullptr)
    {
        printf ("Error openening/creating file.");
        return 0;
    }

    char word[MAX_WORD_SIZE] = {};
    char *record_word;
    int word_size = 0;
    char **words;
    long long unsigned int words_amount = 0;

    while (!feof (in_file))
    {
        if (get_next_word(in_file, word, current_locale) > 0)
        {
            words_amount++;
        }
    }

    fseek (in_file, 0, SEEK_SET);
    words = (char **)calloc (words_amount, sizeof(char *));
    words_amount = 0;

    while (!feof (in_file))
    {
        for (int i = 0; i < MAX_WORD_SIZE; i++)
        {
            word[i] = 0;
        }
        word_size = get_next_word (in_file, word, current_locale);
        if (word_size > 0)
        {
            record_word = (char *)calloc (word_size + 1, sizeof(char));
            strncpy (record_word, word, word_size);
            words[words_amount] = record_word;
            words_amount++;
        }
    }

    TEST(1);
    TEST(2);
    TEST(3);
    TEST(4);
    TEST(5);
    TEST(6);

    for (int i = 0; i < word_size; i++)
    {
        free (words[i]);
    }
    fcloseall();

    #ifdef _DEBUG
        system ("pause");
    #endif
    return 0;
}

unsigned int HashFunc1(HashTableItem_t item)
{
    return 1;
}

unsigned int HashFunc2(HashTableItem_t item)
{
    return get_ru_code (**item);
}

unsigned int HashFunc3(HashTableItem_t item)
{
    return strlen (*item);
}

unsigned int HashFunc4(HashTableItem_t item)
{
    int length = strlen(*item);
    unsigned int sum = 0;

    for (int i = 0; i < length; i++)
    {
        sum += get_ru_code ((*item)[i]);
    }

    return sum;
}

unsigned int HashFunc5(HashTableItem_t item)
{
    int length = strlen(*item);
    unsigned int h = 0;

    for (int i = 0; i < length; i++)
    {
        h = (h << 1) ^ (get_ru_code ((*item)[i]));
    }

    return h;
}

unsigned int HashFunc6(HashTableItem_t item)
{
    int length = strlen(*item);
    unsigned int h = 0;

    for (int i = 0; i < length; i++)
    {
        h = (_rotl(h, 1)) ^ (get_ru_code ((*item)[i]));
    }
    return h;
}

int get_next_word (FILE *in_file, char* word, _locale_t locale)
{
    int c = 0;
    int word_size = 0;
    while (true)
    {
        c = fgetc (in_file);
        if (_isalpha_l (c, locale) == 0)
        {
            return word_size;
        }
        else
        {
            c = _tolower_l(c, locale);
            word[word_size++] = c;
        }
    }
}

unsigned int get_ru_code (char c)
{
    return 256 + (int)c;
}

bool hashtable_nums_to_csv (HashTable_t *hashTable, FILE *out_file)
{
    if (hashTable == nullptr || out_file == nullptr) return false;

    for (int i = 0; i < PRIME_SIZE; i++)
    {
        fprintf (out_file, "%d;%d\n", i, hashTable->lists[i]->num);
    }

    return true;
}

void run_test (FILE *out_file, long long int words_amount, _locale_t current_locale, unsigned int (*HashFunc)(HashTableItem_t item), char **words)
{
    HashTable_t *hashTable = (HashTable_t *)calloc (1, sizeof(HashTable_t)); 
    HashTable_ctor (hashTable, HashFunc);

    for (int i = 0; i < words_amount; i++)
    {
        HashTable_add (hashTable, &(words[i]));
    }

    hashtable_nums_to_csv (hashTable, out_file);

    HashTable_dtor (hashTable);
    free (hashTable);
    hashTable = nullptr;
}