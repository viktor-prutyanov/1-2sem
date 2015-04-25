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

#include "ListStack.h"

class PostfixExpression
{
public:
    PostfixExpression(std::string infixStr);
    ~PostfixExpression();
    std::string Str();
private:
    ListStack<int> stack;
    std::string postfixStr;
};

PostfixExpression::PostfixExpression(std::string infixStr) 
{
    ListStack<int> stack;
    std::string postfixStr;
}