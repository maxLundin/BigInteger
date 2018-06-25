#pragma once

#include <iostream>
#include <vector>
#include "OptimizedArrayList.h"
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

struct big_integer {

    friend std::ostream &operator<<(std::ostream &, big_integer const&);

    friend std::istream &operator>>(std::istream &, big_integer &);

    void cutBadZero();

    std::string to_string() const;

    friend std::string to_string(big_integer const &a);

    bool compare_without_sign_and_equals(const big_integer &);

    unsigned int div_long_short(unsigned int number);

    unsigned int div_long_by_10();

    void mul_long_short(unsigned int number);

    void add_long_short(unsigned int number);

    big_integer reverseIt();

    big_integer();

    big_integer(big_integer const &other);

    big_integer(int a);

    big_integer(size_t a, bool);

    explicit big_integer(std::string const &str);

    ~big_integer() = default;

    big_integer(big_integer &&) noexcept;

    big_integer &operator/=(big_integer const &rhs);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator+=(const big_integer &);

    big_integer &operator*=(const big_integer &);

    big_integer &operator-=(const big_integer &);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    big_integer &operator<<=(short shift);

    big_integer &operator>>=(short shift);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~();

    big_integer &operator++();

    big_integer &operator--();

    big_integer &operator=(big_integer const &other);

    short compare(const big_integer &other) const;

    static void divide(big_integer &res, const big_integer &a, const big_integer &b);

    static void sub_equal(big_integer &a, const big_integer &b);

    std::vector<unsigned int> digits;
//    OptimizedArrayList digits;
    int sign;

};

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer const &a, big_integer const &b);

big_integer operator/(big_integer const &a, big_integer const &b);

big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);

big_integer operator|(big_integer a, big_integer const &b);

big_integer operator^(big_integer a, big_integer const &b);

big_integer operator<<(big_integer a, int b);

big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const &a, big_integer const &b);

bool operator!=(big_integer const &a, big_integer const &b);

bool operator<(big_integer const &a, big_integer const &b);

bool operator>(big_integer const &a, big_integer const &b);

bool operator<=(big_integer const &a, big_integer const &b);

bool operator>=(big_integer const &a, big_integer const &b);
