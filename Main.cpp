#include "big_integer.h"
#include <iostream>

int main()
{
	big_integer a = 2147483647;
	big_integer b = -2147483648;
	std::cout << a << b;
    big_integer c =  a + b;
	std::cout << c;
	return 0;
}
