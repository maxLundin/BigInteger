
#include "BigInteger.cpp"
#include <iostream>

int main()
{
    BigInteger a, b;
	std::cin >> a >> b;
	//std::cout << a << std::endl;
    BigInteger c = a * b;
	std::cout << c << std::endl;
	BigInteger s;
	std::cin >> s;
	s = s << 10;		
	return 0;
}
