/**
*   Stack (LIFO) data structure. Stack interface.
*
*   @file IStack.h
*
*   @date 05.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

//TODO: Add comments.

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <vector>

using std::vector;

template <class T>
class IStack
{
public:
    virtual void Push(T value) = 0;
    virtual T Pop() = 0;
    virtual bool Ok() = 0;
    virtual bool Dump(std::ostream *dump_stream) = 0;
    virtual bool IsEmpty() = 0;
    virtual size_t GetSize() = 0;
    virtual size_t GetCount() = 0;
};