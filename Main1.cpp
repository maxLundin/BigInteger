#include <iostream>

#include "big_integer.cpp"

int main() {
    big_integer a = 100000000000000000;
    big_integer b = 100;
    big_integer c;
	c = a/b;
    std::cout << c << std::endl;
    return 0;
}
