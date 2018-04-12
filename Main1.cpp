#include <iostream>

#include "big_integer.cpp"

int main() {
    big_integer a = 100;
    big_integer b = 100;
    big_integer c = 200;
    bool f = (a != c);
    std::cout << f << std::endl;
    return 0;
}
