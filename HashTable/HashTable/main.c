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
#include <process.h>

#define MAX_WORD_SIZE 64

typedef struct test_params_t
{
    FILE *out_file;
    long long int words_amount; 
    unsigned int (*HashFunc)(HashTableItem_t item); 
    char **words;
    bool thread_flag;
    unsigned int thread_number;
};

unsigned int get_ru_code (char c);
int get_next_word (FILE *in_file, char* word, _locale_t locale);
long long unsigned int get_words_amount(FILE *in_file, _locale_t current_locale);

void run_test(void *params);
//void run_test (FILE *out_file, long long int words_amount, _locale_t current_locale, unsigned int (*HashFunc)(HashTableItem_t item), char **words);
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
    
    FILE *in_file = fopen ("..\\12.txt", "r");

    if (in_file == nullptr)
    {
        printf ("Error openening file.\n");
        return 0;
    }

    FILE *out_file1 = fopen ("..\\csv\\1.csv", "w");
    FILE *out_file2 = fopen ("..\\csv\\2.csv", "w");
    FILE *out_file3 = fopen ("..\\csv\\3.csv", "w");
    FILE *out_file4 = fopen ("..\\csv\\4.csv", "w");
    FILE *out_file5 = fopen ("..\\csv\\5.csv", "w");
    FILE *out_file6 = fopen ("..\\csv\\6.csv", "w");

    if (out_file1 == nullptr || out_file2 == nullptr || out_file3 == nullptr || out_file4 == nullptr || out_file5 == nullptr || out_file6 == nullptr)
    {
        printf ("Error openening/creating file.\n");
        return 0;
    }
    
    printf ("File is opened.\n");

    char word[MAX_WORD_SIZE] = {};
    char **words;
    int word_size = 0;
    long long unsigned int words_amount = 0;

    printf ("Amount of words in file is: %lld\n", words_amount = get_words_amount (in_file, current_locale));

    words = (char **)calloc (words_amount, sizeof(char *));

    for (int i = 0; i < words_amount; i++)
    {
        words[i] = (char *)calloc (MAX_WORD_SIZE, sizeof(char));
    }

    words_amount = 0;

    while (!feof (in_file))
    {
        word_size = get_next_word (in_file, words[words_amount], current_locale);
        if (word_size > 0) words_amount++;
    }

    test_params_t params1 = {out_file1, words_amount, HashFunc1, words, false, 1};
    test_params_t params2 = {out_file2, words_amount, HashFunc2, words, false, 2};
    test_params_t params3 = {out_file3, words_amount, HashFunc3, words, false, 3};
    test_params_t params4 = {out_file4, words_amount, HashFunc4, words, false, 4};
    test_params_t params5 = {out_file5, words_amount, HashFunc5, words, false, 5};
    test_params_t params6 = {out_file6, words_amount, HashFunc6, words, false, 6};

    _beginthread (&run_test, 0, &params1);
    _beginthread (&run_test, 0, &params2);
    _beginthread (&run_test, 0, &params3);
    _beginthread (&run_test, 0, &params4);
    _beginthread (&run_test, 0, &params5);
    run_test (&params6);

    while (!(params1.thread_flag && params2.thread_flag && params3.thread_flag && params4.thread_flag && params5.thread_flag));

    printf ("All threads finished.\n");
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
        h = (_rotl (h, 1)) ^ (get_ru_code ((*item)[i]));
    }
    return h;
}


int get_next_word(FILE *in_file, char* word, _locale_t locale)
{
    if (in_file == nullptr || word == nullptr) return -1;
    int c = 0;
    int word_size = 0;

    while (!feof (in_file))
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
    return word_size;
}

unsigned int get_ru_code(char c)
{
    return 256 + (int)c;
}

long long unsigned int get_words_amount(FILE *in_file, _locale_t current_locale)
{
    if (in_file == nullptr || current_locale == nullptr) return 0;

    int c = 0;
    unsigned int  word_size = 0;
    long long unsigned int words_amount = 0;

    fseek (in_file, 0, SEEK_SET);

    while (!feof (in_file))
    {
        word_size = 0;
        while (!feof (in_file))
        {
            c = fgetc (in_file);
            if (_isalpha_l (c, current_locale) == 0)
            {
                break;
            }
            else
            {
                word_size++;
            }
        }
        if (word_size > 0) words_amount++;
    }

    fseek (in_file, 0, SEEK_SET);
    return words_amount;
}

bool hashtable_nums_to_csv(HashTable_t *hashTable, FILE *out_file)
{
    if (hashTable == nullptr || out_file == nullptr) return false;

    for (int i = 0; i < PRIME_SIZE; i++)
    {
        fprintf (out_file, "%d;%d\n", i, hashTable->lists[i]->num);
    }

    return true;
}

void run_test(void *params)
{
    printf ("Thread #%d started.\n", ((test_params_t *)params)->thread_number);
    HashTable_t *hashTable = (HashTable_t *)calloc (1, sizeof(HashTable_t)); 
    HashTable_ctor (hashTable, ((test_params_t *)params)->HashFunc);

    for (int i = 0; i < ((test_params_t *)params)->words_amount; i++)
    {
        HashTable_add (hashTable, &(((test_params_t *)params)->words[i]));
    }

    hashtable_nums_to_csv (hashTable, ((test_params_t *)params)->out_file);

    //HashTable_dump (hashTable);

    HashTable_dtor (hashTable);
    free (hashTable);
    hashTable = nullptr;
    printf ("Thread #%d finished.\n", ((test_params_t *)params)->thread_number);
    ((test_params_t *)params)->thread_flag = true;
    if (((test_params_t *)params)->thread_number != 6)
    {
        _endthread ();
    }
}