/**
*   Lexical analyzer
*
*   @date 11.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include "lex.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *Cur_sym;

ScanResult_t GetNextToken(Token_t *token)
{
    char func_name[8] = {};
    if (isalpha (*Cur_sym))
    {
        if (isalpha(*(Cur_sym + 1)))
        {
            int i = 0;
            while (*Cur_sym != '(' && *Cur_sym != ' ')
            {
                func_name[i] = *Cur_sym++;
                i++;
            }
            if (false)
            {
                
            }
            #define FUNC(name, str, num) else if (strcmp(str, func_name) == 0) {token->value = num;}
            #include "funcs.h"
            #undef FUNC
            else
            {
                return ERROR;
            }
            token->type = FUNCTION;
            return OK;
        }
        else
        {
            token->type = VARIABLE;
            token->value = *Cur_sym++;
            return OK;
        }
    }
    else if (isdigit (*Cur_sym))
    {
        int val = 0;
        while (true)
        {
            if ('0' <= *Cur_sym && *Cur_sym <= '9')
            {
                val = val * 10 + *Cur_sym++ - '0';
            }
            else
            {
                break;
            }
        }
        token->type = NUMBER;
        token->value = val;
        return OK;
    }
    else if (*Cur_sym == '\0') 
    {
        token->type = CONTROL;
        token->value = *Cur_sym;
        return END;
    }
    else if (strchr ("+-/*^", *Cur_sym) != nullptr)
    {
        token->type = OPERATOR;
        token->value = *Cur_sym++;
        return OK;
    }
    else if (strchr ("<>=!", *Cur_sym) != nullptr) 
    {
        token->type = OPERATOR;
        if (*(Cur_sym + 1) == '=')
        {
            if (false)
            {

            }
            #define OPER(name, str, num) else if (str[0] == *Cur_sym) {token->value = num;}
            #include "opers.h"
            #undef OPER
            else
            {
                return ERROR;
            }
            Cur_sym++;
            Cur_sym++;
        }
        else
        {
            token->value = *Cur_sym++;
        }        
        return OK;
    }
    else if (strchr ("(){},;", *Cur_sym) != nullptr)
    {
        token->type = CONTROL;
        token->value = *Cur_sym++;
        return OK;
    }
    else if (strchr ("\t\n", *Cur_sym) != nullptr)
    {
        token->type = NO_TYPE;
        token->value = *Cur_sym++;
        return OK;
    }
    else if (strchr (" ", *Cur_sym) != nullptr)
    {
        token->type = NO_TYPE;
        token->value = *Cur_sym++;
        return OK;
    }
    else
    {
        return ERROR;
    }
}

ScanResult_t Scan(char *text, Token_t *tokens)
{
    Cur_sym = text;
    Token_t *cur_token = tokens;
    ScanResult_t res = OK;
    size_t amount = 0;

    while (res == OK)
    {
        res = GetNextToken (cur_token);
        if (cur_token->type != NO_TYPE)
        {
            *cur_token++;
        }
    }

    if (res == END)
    {
        return OK;
    }
    else
    {
        return ERROR;
    }
}

void Token_clear(Token_t *token)
{
    token->type = NO_TYPE;
    token->value = -1;
}

size_t GetTokensAmount(char *text)
{
    Cur_sym = text;
    Token_t *token = (Token_t *)calloc (1, sizeof (Token_t));
    ScanResult_t res = OK;
    size_t amount = 0;

    while (res == OK)
    {
        Token_clear (token);
        res = GetNextToken (token);
        //printf ("%d type=%d val=%d val=\'%c\'\n", res, token->type, token->value, token->value);
        if (token->type != NO_TYPE)
        {
            amount++;
        }
    }

    if (res == END)
    {
        return amount;
    }
    else
    {
        return 0;
    }
}