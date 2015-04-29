/**
*   Postfix/infix math expression class.
*
*   @file Expression.cpp
*
*   @date 04.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#include <string>
#include <stdexcept>
#include <iostream>

#include "ExprToken.h"

class Expression
{
public:
    Expression(const char *infixStr);
    ~Expression();
    const char *InfixStr();
    const char *PostfixStr();
    size_t GetPostfixStrLen();
    size_t GetInfixStrLen();
    bool Convert();
    void Dump();
private:
    ExprToken *getNextToken(const char *&ptr);
    TokenPtrListStack stack;
    char *postfixStr;
    const char *infixStr;
    size_t infixStrLen;
    size_t postfixStrLen;
};

Expression::Expression(const char *infixStr)
   :stack (TokenPtrListStack()),
    postfixStr (nullptr),
    infixStr (infixStr) 
{
    if (infixStr == nullptr)
    {
        throw std::invalid_argument("Nullptr as postfix expression string.");
    }

    infixStrLen = strlen(infixStr);
    postfixStrLen = infixStrLen * 2; //FIXME: Too many memory will be allocated!
    postfixStr = new char[postfixStrLen];
    memset(postfixStr, 0, postfixStrLen);
}

const char *Expression::InfixStr()
{
    return infixStr;
}

const char *Expression::PostfixStr()
{
    return (const char *)postfixStr;
}

size_t Expression::GetInfixStrLen()
{
    return infixStrLen;
}

size_t Expression::GetPostfixStrLen()
{
    return postfixStrLen;
}

Expression::~Expression()
{
    while(!stack.IsEmpty())
    {
        delete stack.Pop();
    }
    delete[] postfixStr;
}

bool Expression::Convert()
{
    const char *infixPtr = infixStr;
    char *postfixPtr = postfixStr;

    ExprToken *token1, *token2;

    while (infixPtr != infixStr + infixStrLen)
    {
        token1 = getNextToken(infixPtr);
	    if (token1->Len == 0)
        {
            delete token1;
            return false;
        }

        switch (token1->Type)
        {
        case ExprTokenType::NUMBER:
            memcpy(postfixPtr, infixPtr - token1->Len, token1->Len);
            postfixPtr += token1->Len;
            delete token1;
            break;
        case ExprTokenType::ADD:
        case ExprTokenType::SUB:
        case ExprTokenType::MUL:
        case ExprTokenType::DIV:
        case ExprTokenType::POW:
            while (!stack.IsEmpty() && stack.Peak()->Priority >= token1->Priority)
            {
                token2 = stack.Pop();
                memcpy(postfixPtr, token2->Str, token2->Len);
                postfixPtr += token2->Len;
                delete token2;
            }
            stack.Push(token1);
            break;
        case ExprTokenType::OPENING_BR:
            stack.Push(token1);
            break;
        case ExprTokenType::CLOSING_BR:
            while (stack.Peak()->Type != ExprTokenType::OPENING_BR)
            {
                if (stack.IsEmpty()) return false;
                token1 = stack.Pop();
                memcpy(postfixPtr, token1->Str, token1->Len);
                postfixPtr += token1->Len;
                delete token1;
            }
            
            if (stack.IsEmpty()) return false;
            delete stack.Pop();
            break;
        default:
	        return false;
            break;
        }
    }

    while (!stack.IsEmpty())
    {
        token1 = stack.Pop();
        memcpy(postfixPtr, token1->Str, token1->Len);
        postfixPtr += token1->Len;
        delete token1;
    }

    return true;
}

void Expression::Dump()
{
    const char *infixPtr = infixStr;
    while (infixPtr != infixStr + infixStrLen)
    {
        ExprToken *token = getNextToken(infixPtr);
        if (token->Len == 0)
        {
            printf("Something went wrong.\n");
            delete token;
            return;
        }
        fputs("|", stdout);
        fwrite(token->Str, token->Len, sizeof(char), stdout);
        fputs("| ", stdout);
        delete token;
    }
    fputs("\n", stdout);
}

ExprToken *Expression::getNextToken(const char *&ptr)
{
    while (*ptr == ' ') ++ptr;

    const char *old_ptr = ptr;
    switch (*ptr)
    {
    case '0' ... '9':
        while ('0' <= *ptr && *ptr <= '9') ++ptr;
        return new ExprToken(old_ptr, ptr - old_ptr, ExprTokenType::NUMBER, 0);
        break;
    case '+':
        return new ExprToken(ptr++, 1, ExprTokenType::ADD, 1);
        break;
    case '-':
        return new ExprToken(ptr++, 1, ExprTokenType::SUB, 1);
        break;
    case '*':
        return new ExprToken(ptr++, 1, ExprTokenType::MUL, 2);
        break;
    case '/':
        return new ExprToken(ptr++, 1, ExprTokenType::DIV, 2);
        break;
    case '^':
        return new ExprToken(ptr++, 1, ExprTokenType::POW, 3);
        break;
    case '(':
        return new ExprToken(ptr++, 1, ExprTokenType::OPENING_BR, 0);
        break;
    case ')':
        return new ExprToken(ptr++, 1, ExprTokenType::CLOSING_BR, 0);
        break;
    default:
        return new ExprToken(nullptr, 0, ExprTokenType::NO_TYPE, 0);
        break;
    }
    return nullptr;
}
