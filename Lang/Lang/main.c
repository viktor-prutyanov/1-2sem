/**
*   Compiler
*
*   @date 12.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "gen.h"

size_t file_length(FILE *file)
{
    if (file == nullptr) return 0;
    fseek (file, 0, SEEK_END);
    size_t length = ftell (file);
    rewind (file);
    return length;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n\tlang <file.src>\n");
        return EXIT_FAILURE;
    }
    FILE *log_file = fopen ("1.log", "w");
    FILE *in_file  = fopen (argv[1], "r");
    FILE *asm_file = fopen ("1.asm", "w");
    if (in_file == nullptr || log_file == nullptr || asm_file == nullptr)
    {
        printf ("Error opening file.\n");
        return EXIT_FAILURE;
    }

    fprintf (log_file, "Lexical analysis:\n");
    size_t length = file_length(in_file);
    fprintf (log_file, "Total %d symbols.\n", length);
    char *src_text = (char *)calloc (length + 1, sizeof (char));
    fread ((void *)src_text, sizeof (char), length, in_file); 
    src_text[length] = '\0';

    size_t tokens_amount = GetTokensAmount (src_text);
    fprintf (log_file, "Total %d tokens.\n", tokens_amount);
    Token_t *tokens = (Token_t *)calloc (tokens_amount, sizeof (Token_t));
    Scan (src_text, tokens);
    free (src_text);

    Token_t *token = tokens;
    for (int i = 0; i < tokens_amount; i++)
    {
        fprintf (log_file, "%2d) type = %d value = %3d \n", token - tokens, token->type, token->value);
        token++;
    }

    fprintf (log_file, "\nSyntax analysis:\n");

    Tree_t *tree = (Tree_t *)calloc (1, sizeof (Tree_t));

    Tree_ctor (tree);
    
    GetG0 (tokens, tree, tokens_amount);
    extern bool Err;
    if (Err)
    {
        printf ("Error, see logfile.\n");
        fprintf (log_file, "\nSyntax analysis error.\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return EXIT_FAILURE;
    }

    Tree_print_prefix (tree, log_file);
    fprintf (log_file, "\n");
    Tree_print_infix (tree, log_file);
    fprintf (log_file, "\n");

    if (!Generate (tree->root, asm_file))
    {
        printf ("Error, see logfile.\n");
        fprintf (log_file, "\nGeneration error.\n");
        #ifdef _DEBUG
            system ("pause");
        #endif
        return EXIT_FAILURE;
    }
    fprintf (asm_file, "end");

    fprintf (log_file, "Generation successfully completed.\n");

    fcloseall ();
    log_file = nullptr;
    in_file  = nullptr;
    asm_file = nullptr;

    Tree_dtor (tree);
    free (tree);
    tree = nullptr;

    printf ("Compilation successfully completed.\n");

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
}