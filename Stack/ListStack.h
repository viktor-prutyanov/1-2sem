/**
*   Stack (LIFO) data structure via std::list.
*
*   @file ListStack.h
*
*   @date 04.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

//TODO: Add comments.

#include <stdexcept>
#include <iostream>
#include <list>
#include <algorithm>

#include "IStack.h"

using std::list;

template <class T>
class ListStack : public IStack<T>
{
public:
    ListStack();
    ListStack(const ListStack<T>& stack);
    ~ListStack();
    void Push(T value);
    T Pop();
    T Peak();
    bool Ok();
    bool Dump(std::ostream *dump_stream);
    bool IsEmpty();
    size_t GetSize();
    size_t GetCount();
private:
    list<T> l_data;
};

template <class T>
ListStack<T>::ListStack() 
    : l_data (list<T>())
{
    //Nothing to do
}

template <class T>
ListStack<T>::ListStack(const ListStack<T>& stack)
    : l_data (list<T>(stack.l_data))
{
    //Nothing to do
}

template <class T>
ListStack<T>::~ListStack()
{
    //Nothing to do
}

template <class T>
void ListStack<T>::Push(T value)
{
    l_data.push_back(value);
}

template <class T>
T ListStack<T>::Pop()
{
    if (l_data.size() == 0)
    {
        throw std::out_of_range("Attemp to pop from empty stack. Nothing is done.");
        return Pop();
    }
    T back = l_data.back();
    l_data.pop_back();
    return back;
}

template <class T>
T ListStack<T>::Peak()
{
    if (l_data.size() == 0)
    {
        throw std::out_of_range("Attemp to get peak of empty stack. Nothing is done.");
        return Peak();
    }
    return l_data.back();
}

template <class T>
bool ListStack<T>::Ok()
{
    return true;
}

template <typename T>
bool ListStack<T>::Dump(std::ostream *dump_stream)
{
    *dump_stream << "Stack is OK. \n  Size = " << l_data.size() << "\n";
    size_t i = 0;
    std::for_each(l_data.begin(), l_data.end(), [dump_stream, &i](const T& item) 
        {
            *dump_stream << "\t[" << i << "] " << item << "\n";
            i++;
        });
    return true;
}

template <class T>
bool ListStack<T>::IsEmpty()
{
    return l_data.empty();
}

template <class T>
size_t ListStack<T>::GetCount()
{
    return l_data.size();
}

template <class T>
size_t ListStack<T>::GetSize()
{
    return l_data.size();
}