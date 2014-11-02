/**
*   Stack (LIFO) data structure.
*
*   @file stack.h
*
*   @date 10.2014
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com 
*/

#pragma once

#ifdef _DEBUG
    #define VERIFY(stack)                                                                                       \
        if (!Stack_ok (stack))                                                                                  \
        {                                                                                                       \
            printf("Error in %s on %d in function %s line:", __FILE__, __LINE__, __FUNCTION__);                 \
            Stack_dump(stack);                                                                                  \
            abort();                                                                                            \
        }
#else
    #define VERIFY(stack) ((void)0)
#endif

typedef double element_t;

typedef struct Stack_t
{
    element_t *data;
    size_t count;
    size_t size;
};

/**
    @brief Constructor for Stack_t

    @param size is size of stack
    @param[out] pointer to new stack

    @return true if everything is stack was created, false if not 
*/
bool Stack_ctor(size_t size, Stack_t *stack);

/**
    @brief Destructor for Stack_t

    @param stack is pointer to stack you want do destruct

    @return false if destruction was succesful, true if unsuccessfull
*/
bool Stack_dtor(Stack_t *stack);

/**
    @brief Pops the value from stack

    @param stack is pointer to stack
    @param[out] success is pointer where will be write true is success or false if not

    @return popped value
*/
double Stack_pop(Stack_t *stack, bool *success);

/**
    @brief Pushes the value into stack
    @warning If stack will full the realloc will be called to add 1 item and function may be slowed.

    @param stack is pointer to stack
    @param value is value to push

    @return false if pushing successfull, true if pushing unsuccessfull
*/
bool Stack_push(Stack_t *stack, double value);

/**
    @brief Checks integrity of stack
    @warning If function returns 1 it doesn't meen that EVERYTHING is ok, it means that you can continue to work with it.

    @param stack is pointer to stack you want to check

    @return false if stack is guaranteed to be corrupted, true if you can continue to work with this stack
*/
bool Stack_ok(Stack_t *stack);

/**
    @brief Prints the dump of stack

    @param stack is pointer to stack

    @return true if dump was printed, false if not
*/
bool Stack_dump(Stack_t *stack);

/**
    @brief Checks is stack full.

    @param stack is pointer to stack

    @return true if stack is full, false if not
*/
bool Stack_full(Stack_t *stack);

/**
    @brief Checks is stack empty.

    @param stack is pointer to stack

    @return true if stack is empty, false if not
*/
bool Stack_empty(Stack_t *stack);

/**
    @brief Gets size of stack

    @param stack is pointer to stack

    @return size of stack
*/
size_t Stack_get_size(Stack_t *stack);

/**
    @brief Gets count of elements in stack

    @param stack is pointer to stack

    @return count of stack
*/
size_t Stack_get_count(Stack_t *stack);

/**
    @brief Gets the top value from stack (not pop)

    @param stack is pointer to stack
    @param[out] success is pointer where will be write true is success or false if not

    @return top value
*/
element_t Stack_peak(Stack_t *stack, bool *success);