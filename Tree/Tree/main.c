/**
*   Binary tree
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include "tree.h"
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>

#define ENCODING 1251
#define MAX_WORD_SIZE 64

/**
    @brief Gets length of file

    @param file is pointer to file

    @return length of file in bytes
*/
unsigned long int file_length(FILE *file);

/**
    @brief Function that writes lines to string

    @param in_file is input file
    @param length[out] is length of file
    @param read_length[out] is length of string

    @return string with lines
*/
char *read_file_to_string(FILE *in_file, long unsigned int *length, long unsigned int *read_length);

/**
    @brief Game function

    @param tree is tree for game
    @param out_filename1 is file with tree
    @param out_filename2 is file with pharases
    @param phrases_amount is amount of phrases

    @return true is success, otherwise false
*/
bool play(Tree_t *tree, char *out_filename1, char *out_filename2, char **phrases, long unsigned int phrases_amount);

bool TreeNode_print_phrases(TreeNode_t *node, FILE *file, char **phrases);
bool Tree_print_phrases(Tree_t *tree, FILE *file, char **phrases);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf ("usage: tree\ncommands:\n\tplay [file1] [file2]\n\tprint [file1] [file2]\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return EXIT_FAILURE;
    }

    SetConsoleOutputCP(ENCODING);
    SetConsoleCP(ENCODING);

    FILE *in_file1 =  fopen (argv[2], "r");
    FILE *in_file2 =  fopen (argv[3], "r");

    if (in_file1 == nullptr || in_file2 == nullptr)
    {
        printf ("Error opening file.\n");
        return EXIT_FAILURE;
    }

    long unsigned int length = 0;
    long unsigned int read_length = 0;
    char *text = read_file_to_string (in_file2, &length, &read_length);
    fclose (in_file2);
    in_file2 = nullptr;

    char** phrases = (char **)calloc (length, sizeof(char *));
    phrases[0] = text;
    long unsigned int phrases_amount = 0;

    for (int i = 0; i < read_length; i++)
    {
        if (text[i] == '\n') 
        {
            text[i] = '\0';
            phrases[++phrases_amount] = &(text[i+1]);
        }
    }

    Tree_t *tree = (Tree_t *)calloc (1, sizeof(Tree_t));
    Tree_ctor (tree);
    Tree_read (tree, in_file1);
    fclose (in_file1);
    in_file1 = nullptr;

    printf ("Length: %u(%u) Phrases amount: %u\n", length , read_length, phrases_amount);

    if (strcmp(argv[1], "play") == 0)
    {
        play (tree, argv[2], argv[3], phrases, phrases_amount);

    }
    else if (strcmp(argv[1], "print") == 0)
    {
        Tree_print (tree, stdout);
        printf ("\n");
        Tree_print_phrases (tree, stdout, phrases);
    }
    else
    {
        printf ("usage: tree\ncommands:\n\tplay\n\tprint\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return EXIT_FAILURE;
    }

    printf ("\n");
    Tree_dtor (tree);
    free (tree);
    tree = nullptr;

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
};

unsigned long int file_length(FILE *file)
{
    if (file == nullptr) return 0;
    fseek (file, 0, SEEK_END);
    unsigned long int length = ftell (file);
    fseek (file, 0, SEEK_SET);
    return length;
}

char *read_file_to_string(FILE *in_file, long unsigned int *length, long unsigned int *read_length)
{
    if (in_file == nullptr) return nullptr;
    *length = file_length (in_file); //unsigned long int length = _filelength(_fileno(file)) + 1;
    char *buf = (char *)calloc (*length + 1, sizeof(char));
    if (buf == nullptr) return nullptr;
    *read_length = fread (buf, sizeof(char), *length, in_file);
    return buf;
}

bool play(Tree_t *tree, char *out_filename1, char *out_filename2, char **phrases, long unsigned int phrases_amount)
{
    if (tree == nullptr || out_filename1 == nullptr || out_filename2 == nullptr || phrases == nullptr) return false;
    
    printf ("Welcome to game!\n");
    printf ("Think about MIPT character.\n\n");
    
    TreeNode_t *current_node = tree->root;
    TreeNode_t *previous_node = tree->root;
    char answer[4] = {};
    char new_character[MAX_WORD_SIZE] = {};
    char new_question[MAX_WORD_SIZE] = {};
    int result = 0;

    while (true)
    {
        if (TreeNode_is_leaf(current_node))
        {
            printf ("Your character is %s?(yes|no)\n", phrases[current_node->data]);
            scanf ("%3s", &answer);
            if (answer[0] == 'y')
            {
                printf ("Your answer is yes.\n");
                printf ("Good game, well played!\n");
                return true;
            }
            else if (answer[0] == 'n')
            {
                printf ("Your answer is no.\n");
                printf ("Who is he? (%d symbols max)\n", MAX_WORD_SIZE);
                getchar ();
                gets (new_character);
                printf ("New character is \"%s\"\n", new_character);
                printf ("Question to determine \"%s\"(no) from \"%s\"(yes) (%d symbols max):\n", phrases[current_node->data], new_character, MAX_WORD_SIZE);
                gets (new_question);
                printf ("New question is \"%s\"\n", new_question);

                int num = current_node->data;
                current_node->data = phrases_amount;

                TreeNode_add (tree, TreeNode_new (num, nullptr, nullptr), TreeNode_new (phrases_amount + 1, nullptr, nullptr), current_node); 
                //printf ("%d %d\n", num, phrases_amount + 1);

                FILE *out_file1 =  fopen ("..\\1.txt", "w+");
                FILE *out_file2 =  fopen ("..\\2.txt", "a");

                Tree_print (tree, out_file1);
                fprintf (out_file2, "%s\n", new_question);
                fprintf (out_file2, "%s\n", new_character);

                fclose (out_file1);
                fclose (out_file2);
                return true;
            }
            else
            {
                printf ("Invalid answer.\n");
            }
        }
        printf ("%s(yes|no)\n", phrases[current_node->data]);
        scanf ("%3s", &answer);
        if (answer[0] == 'y')
        {
            printf ("Your answer is yes.\n");
            previous_node = current_node;
            current_node = current_node->right;
        }
        else if (answer[0] == 'n')
        {
            printf ("Your answer is no.\n");
            previous_node = current_node;
            current_node = current_node->left;
        }
        else
        {
            printf ("Invalid answer.\n");
        }
    }
}

bool Tree_print_phrases(Tree_t *tree, FILE *file, char **phrases)
{
    if (tree == nullptr || file == nullptr) return false;
    return TreeNode_print_phrases (tree->root, file, phrases);
}

bool TreeNode_print_phrases(TreeNode_t *node, FILE *file, char **phrases)
{
    if (node == nullptr || file == nullptr) return false;
    fprintf (file, "(%s" , phrases[node->data]);
    if (node->left != nullptr) TreeNode_print_phrases (node->left, file, phrases);
    if (node->right != nullptr) TreeNode_print_phrases (node->right, file, phrases);
    fprintf (file, ")");
    return true;
}