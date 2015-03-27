/**
*   Vector data container.
*
*   @file main.cpp
*
*   @date 05.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/

#include <stdio.h>
#include <cstdlib>
#include <stdexcept>

/**
 * @brief vector data container
 * @details wrapper over array similar to the std::vector
 * 
 * @tparam T is type of elements in vector
 */
template <typename T>
class Vector
{
public:
	Vector();
	Vector(const Vector<T>& vector);
	Vector(size_t size, const T& value);
	~Vector();

	void dump();

    T& operator[](size_t pos);
    T& at(size_t pos);
    T& insert(size_t pos, const T& value);
    size_t resize(size_t count, const T& value);

	size_t capacity();
	size_t size();
    bool empty();
private:
	size_t v_capacity;
	size_t v_size;
	T *v_data;
};

/**
 * @brief default constructor
 * @details constructs vector with memory for one element
 */
template <typename T>
Vector<T>::Vector()
	: v_size(0), v_capacity(1), v_data(new T[1])
{
	//Nothing to do
}

/**
 * @brief copying constructor
 * @details constructs copy of vector
 * 
 * @param vector is vector to copy
 */
template <typename T>
Vector<T>::Vector(const Vector<T>& vector)
	: v_size(vector.v_size), v_capacity(vector.v_capacity), v_data(new T[v_size])
{
	for (size_t i = 0; i < v_size; i++)
	{
		v_data[i] = vector.v_data[i];
	}
}

/**
 * @brief constructor via same values
 * @details constructs vector and fills it with same values
 * 
 * @param size is size of vector
 * @param value is value to fill
 */
template <typename T>
Vector<T>::Vector(size_t size, const T& value)
	: v_size(size), v_capacity(size), v_data(new T[size])
{
	for (size_t i = 0; i < v_size; i++)
	{
		v_data[i] = value;
	}
}

/**
 * @brief destructor
 */
template <typename T>
Vector<T>::~Vector()
{
    delete[] v_data;
}

/**
 * @brief prints dump of vector to stdout
 */
template <typename T>
void Vector<T>::dump()
{
    printf("Vector at 0x%p. Size is %lu, capacity is %lu.\n", this, v_size, v_capacity);
	if (v_size == 0) return;
	for (size_t i = 0; i < v_size; i++)
	{
		printf("%3lu [0x%p] %4d\n", i, (v_data + i), v_data[i]);
	}
}

/**
 * @brief answer is vector empty or not
 * 
 * @return empty or not
 */
template <typename T>
bool Vector<T>::empty()
{
    return (v_size == 0);
}

/**
 * @brief provides access to element
 * @details throws std::out_of_range if position is out of vector bounds
 * 
 * @param pos is position of element
 * @return reference to element at position 
 */
template <typename T>
T& Vector<T>::at(size_t pos)
{
    if (pos < v_size)
    {
        return v_data[pos];
    }
    else
    {
        throw std::out_of_range("Position out of vector bounds.");
    }
}

/**
 * @brief inserts an element to vector
 * @warning if there is no memory to insert reallocation will occur
 * @details throws std::length_error if reallocation fails
 * 
 * @param pos is position to insert
 * @param value is value of new element
 * @return reference to inserted element
 */
template <typename T>
T& Vector<T>::insert(size_t pos, const T& value)
{
    if ((v_capacity == v_size) && (pos <= v_size))
    {
        v_data = (T *)realloc((void *)v_data, v_capacity * 2 * sizeof(T));
        if (v_data == nullptr)
        {
            throw std::length_error("Vector reallocation failed.");
        }
        v_capacity *= 2;
        return insert(pos, value);
    }
    else if ((v_capacity != v_size) && (pos <= v_size))
    {
        for (size_t i = v_size; i > pos; --i)
        {
            v_data[i] = v_data[i - 1];
        }
        v_data[pos] = value;
        v_size++;
        return v_data[pos];
    }
    else
    {
        throw std::length_error("Insertion separately from vector.");
    }
}

/**
 * @brief resizes vector
 * @warning if there is no memory to resize reallocation will occur
 * @details throws std::length_error if reallocation fails
 * 
 * @param count is count of elements you want to place
 * @param value is value of new elements
 * @return new size
 */
template  <typename T>
size_t Vector<T>::resize(size_t count, const T& value)
{
    if (count > v_capacity)
    {
        v_data = (T *)realloc((void *)v_data, count * sizeof(T));
        if (v_data == nullptr)
        {
            throw std::length_error("Vector reallocation failed.");
        }

        for (int i = v_size; i < count; ++i)
        {
            v_data[i] = value;
        }
        v_size = count;
        v_capacity = count;
    }
    else if (v_size < count && count <= v_capacity)
    {
        for (int i = v_size; i < count; i++)
        {
            v_data[i] = value;
        }
        v_size = count;
    }
    return v_size;
}

/**
 * @brief provides access to element by []
 * 
 * @param pos is position of element
 * @return reference to element at position 
 */
template <typename T>
T& Vector<T>::operator[](size_t pos)
{
    return v_data[pos];
}

/**
 * @brief vector size
 * 
 * @return size of vector
 */
template <typename T>
size_t Vector<T>::size()
{
    return v_size;
}