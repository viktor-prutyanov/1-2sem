#include "ListStack.h"

enum class ExprTokenType
{
    NO_TYPE = 0,
    NUMBER = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    POW = 6,
    OPENING_BR = 7,
    CLOSING_BR = 8
};

class ExprToken
{
public:
    ExprToken(const char *str, size_t len, ExprTokenType type, unsigned int priority);
    ~ExprToken();

    const char *Str;
    size_t Len;
    ExprTokenType Type;
    unsigned int Priority;
};

ExprToken::ExprToken(const char *str, size_t len, ExprTokenType type, unsigned int priority)
   :Str (str), Len (len), Type (type), Priority (priority) {}

ExprToken::~ExprToken() {}

class TokenPtrListStack : public ListStack<ExprToken *>
{
public:
    TokenPtrListStack();
    ~TokenPtrListStack();
    bool Dump(std::ostream *dump_stream);
private:
    list<ExprToken *> l_data;
};

TokenPtrListStack::TokenPtrListStack() 
    : l_data (list<ExprToken *>()) {}

TokenPtrListStack::~TokenPtrListStack() {}

bool TokenPtrListStack::Dump(std::ostream *dump_stream)
{
    *dump_stream << "Stack is OK. \n  Size = " << l_data.size() << "\n";
    size_t i = 0;
    std::for_each(l_data.begin(), l_data.end(), [&i](const ExprToken *item) 
        {
            printf("\t[%lu] %lu %*s %u", 
                i, item->Len, item->Len, item->Str, item->Priority);
            i++;
        });
    return true;
}
