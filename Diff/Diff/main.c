/**
*   Expression calculator
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <stdio.h>
#include <stdlib.h>
#include "diff.h"
#include <ctype.h>

#define TEX_BEGIN \
"\\documentclass[12pt]{article}\n\
\\usepackage{ucs}\n\
\\usepackage[utf8x]{inputenc}\n\
\\usepackage[russian]{babel}\n\
\\title{\\LaTeX}\n\
\\date{}\n\
\\author{}\n\
\\begin{document}\n\
\t\\begin{center}\n\
\t\t\\Large\\textbf{Taking the derivative of a function}\n\
\t\\end{center}\n"

#define TEX_BEGIN_MATH "\t\\begin{math}\n" 
#define TEX_END_MATH "\t\\end{math}\n" 

#define TEX_END \
"\\end{document}\n"

#define MAX_EXPR_SIZE 256

#define VAR 'x'

int main()
{
    FILE *out_file = fopen ("out.tex", "w");

    if (out_file == nullptr)
    {
        printf ("Invalid output file.\n");
        return EXIT_FAILURE;
    }

    char expr[MAX_EXPR_SIZE] = {};
    printf ("Enter expression (max length is %d) to differentiate, you can use +-/*^()\n", MAX_EXPR_SIZE);
    scanf ("%s", &expr);
    printf ("Enter variable:\n");
    char var[] = "x";
    scanf ("%1s", var);
    if (isalpha (var[0]))
    {
        printf ("Variable is %c\n", var[0]);
    }
    else
    {
        printf ("Invalid variable.\n");
        return EXIT_FAILURE;
    }

    Tree_t *tree = (Tree_t *)calloc (1, sizeof(Tree_t));
    Tree_ctor (tree);
    
    GetG0 (expr, tree);
    extern bool Err;
    if (Err)
    {
        printf ("There was an error. Differentiation aborted.\n");
    }
    else
    {
        Tree_t *diff_tree = (Tree_t *)calloc (1, sizeof (Tree_t));
        Tree_ctor (diff_tree);

        printf ("Input is :\n");
        Tree_print_for_human (tree, stdout);
        printf ("\n");

        
        fprintf (out_file, TEX_BEGIN);
        fprintf (out_file, "\tLet $f(%c) = $\n", var[0]);
        fprintf (out_file, TEX_BEGIN_MATH);
        fprintf (out_file, "\t\t");
        Tree_print_tex (tree, out_file);
        fprintf (out_file, "\\\\\n");
        fprintf (out_file, TEX_END_MATH);
        fprintf (out_file, "\t\\newline\n");

        fprintf (out_file, "\t\\newline\n\tNext, we will take the derivative $f^{'}_%c(%c)$:\\newline\n", var[0], var[0]);
        fprintf (out_file, "\t\\newline\n\tStep-by-step solution:\\newline\n");
        fprintf (out_file, TEX_BEGIN_MATH);
        fprintf (out_file, "\t\t");
        diff_tree->root = Diff (tree, tree->root, var[0], out_file);
        fprintf (out_file, TEX_END_MATH);

        fprintf (out_file, "\t\\newline\n\tThe derivative is:\\newline\n");
        fprintf (out_file, TEX_BEGIN_MATH);
        fprintf (out_file, "\t\t");
        Tree_print_tex (diff_tree, out_file);
        fprintf (out_file, "\\\\\n");
        fprintf (out_file, TEX_END_MATH);
        fprintf (out_file, "\t\\newline\n\tBut you can see many unnecessary actions.\n");
        
        fprintf (out_file, "\t\\newline\n\tLet us make some simplifications:\\newline\n");
        fprintf (out_file, TEX_BEGIN_MATH);
        Optimize (diff_tree, out_file);
        fprintf (out_file, TEX_END_MATH);

        fprintf (out_file, "\t\\newline\n\tIn this way:\\newline\n");

        fprintf (out_file, "\t\t\\framebox{$f^{'}_%c(%c) = ", var[0], var[0]);
        Tree_print_tex (diff_tree, out_file);
        fprintf (out_file, "$}\\\\");

        fprintf (out_file, "\t\\newline\n\tFurther simplifications reader can hold their own.\\newline\n");
        fprintf (out_file, TEX_END);

        Tree_dtor (diff_tree);
        free (diff_tree);
        diff_tree = nullptr;
    }

    fclose (out_file);

    Tree_dtor (tree);
    free (tree);
    tree = nullptr;

    system ("latex -output-format=pdf out.tex");

    #ifdef _DEBUG
        system ("pause");
    #endif

    return EXIT_SUCCESS;
}