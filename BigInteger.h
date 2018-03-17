#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

class BigInteger {
    friend std::ostream &operator<<(std::ostream &, const BigInteger &);

    friend std::istream &operator>>(std::istream &, BigInteger &);

    void cutBadZero();

    static const int BASE_SIZE = 9;
    static const long long BASE = static_cast<const long long int>(1e9);
    int sign;
    std::vector<unsigned int> digits;
public:
    explicit BigInteger(long long value = 0);

    BigInteger(const BigInteger &);

    BigInteger &operator=(const BigInteger &) = default;

    BigInteger(BigInteger &&);
    //BigInteger();

    BigInteger &operator+=(const BigInteger &);

    BigInteger operator+(const BigInteger &) const;

    BigInteger operator*(const BigInteger &) const;

    BigInteger operator<<(const short shift);

    BigInteger operator>>(const short shift);

    BigInteger &operator-=(const BigInteger &);

    BigInteger operator-(const BigInteger &) const;

    bool operator>(const BigInteger &);
};
