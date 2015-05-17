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

#define MAX_WORD_SIZE 64
#define ASM_HASH 

typedef struct test_params_t
{
	FILE *out_file;
	long long int words_amount;
    unsigned int (__fastcall *HashFunc)(char **);
	char **words;
	bool volatile thread_flag;
	unsigned int thread_number;
};

long get_next_word(FILE *in_file, char* word, _locale_t locale);
long long unsigned int get_words_amount(FILE *in_file, _locale_t current_locale, long unsigned int *len);
void run_test(void *params);
bool hashtable_nums_to_csv(HashTable_t *hashTable, FILE *out_file);

unsigned int __fastcall HashFunc6(char **item);
unsigned int AsmHashFunc6(char **item);

FILE *LOGFILE = stdout;

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	_locale_t current_locale = _get_current_locale();

	if (argc == 3)
	{
		LOGFILE = fopen(argv[2], "w");
		if (LOGFILE == nullptr)
		{
			printf("Error openening log file.\n");
			return 0;
		}
	}
	else if (argc == 2)
	{
		printf("Running without log (LOGFILE == stdout).\n");
	}
	else
	{
		fprintf(LOGFILE, "Incorrect number of arguments.\n");
		return 0;
	}

	FILE *in_file = fopen(argv[1], "r");
	if (in_file == nullptr)
	{
		fprintf(LOGFILE, "Error openening input file.\n");
		return 0;
	}

	FILE *out_file6 = fopen("..\\csv\\6.csv", "w");

	if (out_file6 == nullptr)
	{
		fprintf(LOGFILE, "Error openening/creating file.\n");
		return 0;
	}

	fprintf(LOGFILE, "File is opened.\n");

	int word_size = 0;
	long long unsigned int words_amount = 0;
    long unsigned int len = 0;

	fprintf(LOGFILE, "Amount of words in file is: %lld\n", words_amount = get_words_amount(in_file, current_locale, &len));
    printf("Len is %lu\n", len);

	char** words = (char **)calloc(words_amount, sizeof(char *));

	if (words == nullptr)
	{
		fprintf(LOGFILE, "Memory allocation error.\n");
		return 0;
	}

    fseek(in_file, 0, SEEK_END);
    len = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    printf("Len is %lu\n", len);

	for (int i = 0; i < words_amount; i++)
	{
		words[i] = (char *)calloc(MAX_WORD_SIZE, sizeof(char));
	}

	words_amount = 0;

	while (!feof(in_file))
	{
		word_size = get_next_word(in_file, words[words_amount], current_locale);
		if (word_size > 0) words_amount++;
	}

    test_params_t params6 = { out_file6, words_amount, HashFunc6, words, false, 6 };
	run_test(&params6);

	fprintf(LOGFILE, "All threads finished.\n");
	for (int i = 0; i < word_size; i++)
	{
		free(words[i]);
	}

	fcloseall();

#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}

//unsigned int HashFunc6(char **item)
//{
//    char *str = *item;
//    unsigned int len = strlen(*item);
//    __asm
//    {
//        mov     edx, len
//        mov     edi, str
//        xor     ecx, ecx
//        xor     eax, eax
//hash_loop:
//        rol     eax, 1
//        mov     bl, [edi + ecx]
//        xor     al, bl
//        inc     ecx
//        cmp     ecx, edx
//        jne     hash_loop
//    }
//}

#ifdef ASM_HASH
unsigned int __fastcall HashFunc6(char **item)
{
    __asm
    {
        mov	    edi, [ecx]
        xor     ecx, ecx
        xor     eax, eax
hash_loop:
        mov     bl, [edi + ecx]
        or      bl, bl
        jz      stop
        rol     eax, 1
        xor     al, bl
        inc     ecx
        jmp     hash_loop
stop:
    }
}
#endif // ASM_HASH

#ifndef ASM_HASH
unsigned int __fastcall HashFunc6(char **item)
{
	int length = strlen(*item);
	unsigned int h = 0;
	for (int i = 0; i < length; i++)
	{
		h = (_rotl(h, 1)) ^ ((*item)[i] + 256);
	}
	return h;
}
#endif // !ASM_HASH

long get_next_word(FILE *in_file, char* word, _locale_t locale)
{
	if (in_file == nullptr || word == nullptr) return -1;
	int c = 0;
	long word_size = 0;

	while (!feof(in_file))
	{
		c = fgetc(in_file);
		if (_isalpha_l(c, locale) == 0)
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

long long unsigned int get_words_amount(FILE *in_file, _locale_t current_locale, long unsigned int *len)
{
	if (in_file == nullptr || current_locale == nullptr) return 0;

	int c = 0;
	unsigned int  word_size = 0;
	long long unsigned int words_amount = 0;

	fseek(in_file, 0, SEEK_SET);

	while (!feof(in_file))
	{
		word_size = 0;
		while (!feof(in_file))
		{
			c = fgetc(in_file);
			if (_isalpha_l(c, current_locale) == 0)
			{
                *len += word_size + 1;
				break;
			}
			else
			{
				word_size++;
			}
		}
		if (word_size > 0) words_amount++;
	}

	fseek(in_file, 0, SEEK_SET);
	return words_amount;
}

bool hashtable_nums_to_csv(HashTable_t *hashTable, FILE *out_file)
{
	if (hashTable == nullptr || out_file == nullptr) return false;

	for (int i = 0; i < PRIME_SIZE; i++)
	{
		fprintf(out_file, "%d;%d\n", i, hashTable->lists[i]->num);
	}

	return true;
}

void run_test(void *params)
{
	//fprintf(LOGFILE, "#%d thread started.\n", ((test_params_t *)params)->thread_number);
	HashTable_t *hashTable = (HashTable_t *)calloc(1, sizeof(HashTable_t));
	HashTable_ctor(hashTable, ((test_params_t *)params)->HashFunc);

	for (int i = 0; i < ((test_params_t *)params)->words_amount; i++)
	{
		if (HashTable_search(hashTable, &(((test_params_t *)params)->words[i]), strcmp) < 0)
		{
			HashTable_add(hashTable, &(((test_params_t *)params)->words[i]));
		}
	}

	//hashtable_nums_to_csv(hashTable, ((test_params_t *)params)->out_file);
	//HashTable_dump (hashTable);

	HashTable_dtor(hashTable);
	free(hashTable);
	hashTable = nullptr;
	//fprintf(LOGFILE, "Thread #%d finished.\n", ((test_params_t *)params)->thread_number);
}