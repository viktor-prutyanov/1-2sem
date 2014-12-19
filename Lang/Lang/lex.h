/**
*   Lexical analyzer
*
*   @date 12.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

typedef enum TokenType_t
{
    NO_TYPE =      -1,
    CONTROL =       0,
    A_OPERATOR =    1,
    NUMBER =        2,
    VARIABLE =      3,
    FUNCTION =      4,
    L_OPERATOR =    5,
    W_OPERATOR =    6
};

typedef struct Token_t
{
    int value;
    TokenType_t type;
};

typedef enum ScanResult_t
{
    OK = 0,
    END = -1,
    ERROR = 1
};

ScanResult_t Scan(char *text, Token_t *tokens);
ScanResult_t GetNextToken(Token_t *token);
void Token_clear(Token_t *token);
size_t GetTokensAmount(char *text);