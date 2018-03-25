#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

class BigInteger {

    friend std::ostream &operator<<(std::ostream &, BigInteger &);

    friend std::istream &operator>>(std::istream &, BigInteger &);

    void cutBadZero();

    bool compare_without_sign(const BigInteger&);

    bool compare_without_sign_and_equals(const BigInteger&);

    unsigned int div_long_short(unsigned int number);

    void mul_long_short(unsigned int number);

    void add_long_short(unsigned int number);

    int sign;

public:

    //BigInteger(long long int x);

    std::vector<unsigned int> digits;

    explicit BigInteger(long long value = 0);

    BigInteger(const BigInteger &);

    BigInteger &operator=(const BigInteger &) = default;

    BigInteger(BigInteger &&);

    BigInteger &operator+=(const BigInteger &);

    BigInteger &operator*=(const BigInteger &);

    BigInteger operator+(const BigInteger &) const;

    BigInteger operator*(const BigInteger &) const;

    BigInteger operator<<(short shift);

    BigInteger operator>>(short shift);

    BigInteger &operator<<=(short shift);

    BigInteger &operator>>=(short shift);

    BigInteger operator/(const BigInteger &) const;

    BigInteger operator%(const BigInteger &) const;

    BigInteger &operator-=(const BigInteger &);

    BigInteger operator-(const BigInteger &) const;

    bool operator>(const BigInteger &);

    bool operator==(const BigInteger &);

    bool operator>=(const BigInteger &);

    bool operator<(const BigInteger &);

    bool operator!=(const BigInteger &);

    bool operator<=(const BigInteger &);

};
