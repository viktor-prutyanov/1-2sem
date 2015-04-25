/**
*   Stack (LIFO) data structure via std::vector.
*
*   @file VectorStack.h
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
#include <vector>

#include "IStack.h"

using std::vector;

template <class T>
class VectorStack : public IStack<T>
{
public:
    VectorStack(size_t size);
    VectorStack(const VectorStack<T>& stack);
    ~VectorStack();
    void Push(T value);
    T Pop();
    bool Ok();
    bool Dump(std::ostream *dump_stream);
    bool IsEmpty();
    size_t GetSize();
    size_t GetCount();
private:
    size_t count;
    vector<T> v_data;
};

template <class T>
VectorStack<T>::VectorStack(size_t size) 
    : count (0), v_data (vector<T>(size, 0))
{
    //Nothing to do
}

template <class T>
VectorStack<T>::VectorStack(const VectorStack<T>& stack)
    : count (stack.count), v_data (vector<T>(stack.v_data))
{
    //Nothing to do
}

template <class T>
VectorStack<T>::~VectorStack()
{
    count = 0;
}

template <class T>
void VectorStack<T>::Push(T value)
{
    if (count == v_data.size())
    {
        v_data.push_back(value);
    }
    else
    {
        v_data.at(count) = value;
    }
    count++;
}

template <class T>
T VectorStack<T>::Pop()
{
    if (count == 0)
    {
        throw std::out_of_range("Attemp to pop from empty stack. Nothing is done.");
        return 0;
    }
    return v_data.at(--count);
}

template <class T>
bool VectorStack<T>::Ok()
{
    return !(count > v_data.size());
}

template <class T>
bool VectorStack<T>::Dump(std::ostream *dump_stream)
{
    if (Ok())
    {
        *dump_stream << "Stack is OK. \n\tCount = " << count << " Size = " << v_data.size() << " Capacity = " << v_data.capacity() << "\n";
        for (size_t i = 0; i < count; i++)
        {
            *dump_stream << "\t[" << i << "] " << v_data.at(i) << "\n";
        }
        return true;
    }
    else
    {
        *dump_stream << "Stack is CORRUPTED. \n\tCount = " << count << " Size = " << v_data.size() << " Capacity = " << v_data.capacity() <<  "\n";
        return false;
    }
}

template <class T>
bool VectorStack<T>::IsEmpty()
{
    return (count == 0);
}

template <class T>
size_t VectorStack<T>::GetCount()
{
    return count;
}

template <class T>
size_t VectorStack<T>::GetSize()
{
    return v_data.size();
}