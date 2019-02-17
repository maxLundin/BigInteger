#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include "OptimizedArrayList.h"


struct big_integer {

    big_integer();

    big_integer(big_integer const &other);

    big_integer(int a);

    big_integer(size_t a, bool);

    explicit big_integer(std::string const &str);

    ~big_integer() = default;

    big_integer(big_integer &&) noexcept;

    friend std::ostream &operator<<(std::ostream &, big_integer const &);

    friend std::istream &operator>>(std::istream &, big_integer &);

    std::string to_string() const;

    big_integer &operator/=(big_integer const &rhs);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator+=(const big_integer &);

    big_integer &operator*=(const big_integer &);

    big_integer &operator-=(const big_integer &);

    big_integer &operator<<=(short shift);

    big_integer &operator>>=(short shift);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend big_integer operator+(big_integer a, big_integer const &b);

    friend big_integer operator-(big_integer a, big_integer const &b);

    friend big_integer operator*(big_integer const &a, big_integer const &b);

    friend big_integer operator/(big_integer const &a, big_integer const &b);

    friend big_integer operator%(big_integer a, big_integer const &b);

    friend big_integer operator&(big_integer a, big_integer const &b);

    friend big_integer operator|(big_integer a, big_integer const &b);

    friend big_integer operator^(big_integer a, big_integer const &b);

    friend big_integer operator<<(big_integer a, int b);

    friend big_integer operator>>(big_integer a, int b);

    friend void swap(big_integer &a, big_integer &b);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~();

    big_integer &operator++();

    big_integer &operator--();

    big_integer &operator=(big_integer const &other);

private:
    void cutBadZero();

    big_integer reverseIt();

    void mul_long_short(unsigned int number);

    void add_long_short(unsigned int number);

    friend std::string to_string(big_integer const &a);

    bool compare_without_sign_and_equals(const big_integer &);

    unsigned int div_long_short(unsigned int number);

//    unsigned int div_long_by_10();

    short compare(const big_integer &other) const;

    friend void divide(big_integer &res, const big_integer &a, const big_integer &b);

    friend void sub_equal(big_integer &a, const big_integer &b);

    std::vector<uint32_t > digits;
//    OptimizedArrayList digits;
    int8_t sign;

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

void swap(big_integer &a, big_integer &b);

void divide(big_integer &res, const big_integer &a, const big_integer &b);
