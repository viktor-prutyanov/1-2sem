/**
*   Vector data container. Vector test.
*
*   @file main.cpp
*
*   @date 05.2015
*
*   @copyright GNU GPL v2.0
*
*   @author Viktor Prutyanov mailto:vitteran@gmail.com
*/

#include "vector.h"
#include <iostream>

int main()
{
    Vector<int> vector1;
    vector1.dump();
	Vector<int> vector2(3, 2);
	vector2.dump();
	Vector<int> vector3(vector2);
	vector3.dump();

    printf("%d -> ", vector2.at(2));
    vector2.at(2) = 8;
    printf("%d\n", vector2.at(2));
    vector2.dump();
    vector2[2] = 2;
    vector2.dump();

    printf("Try to access 3rd element of array: ");
    try
    {
        printf("%d\n", vector2.at(3)); 
    }
    catch (const std::out_of_range& e)
    {
        printf("%s\n", e.what());
    }

    vector2.dump();
    vector2.insert(2, 9);
    vector2.insert(2, 8);
    vector2.dump();
    vector2.resize(3, 4);
    vector2.dump();
    vector2.resize(6, 4);
    vector2.dump();
    vector2.insert(6, 50);
    vector2.dump();
    vector2.resize(10, 3);
    vector2.dump();
    vector2.insert(5, 5);
    vector2.dump();
    vector2.insert(vector2.size(), 10);
    vector2.dump();
    vector2.insert(11, 144);
    vector2.dump();
    try
    {
        vector2.insert(15, 123);
    }
    catch (const std::length_error& e)
    {
        printf("%s\n", e.what());
    }
    vector2.dump();

	return 0;
}