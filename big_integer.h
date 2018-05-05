#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

struct big_integer {

    friend std::ostream &operator<<(std::ostream &, big_integer &);

    friend std::istream &operator>>(std::istream &, big_integer &);

    void cutBadZero();

    std::string to_string();

    friend std::string to_string(big_integer const &a);

    bool compare_without_sign_and_equals(const big_integer &);

    unsigned int div_long_short(unsigned int number);

    void mul_long_short(unsigned int number);

    void add_long_short(unsigned int number);

    big_integer reverseIt();

    int sign;

    big_integer();

    big_integer(big_integer const &other);

    big_integer(int a);

    big_integer(size_t a, bool);

    explicit big_integer(std::string const &str);

    ~big_integer() = default;

    big_integer(big_integer &&) noexcept ;

    std::vector<unsigned int> digits;

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

    void divide(big_integer &res, const big_integer &a, const big_integer &b);

    void sub_equal(big_integer &a, const big_integer &b);
};

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer a, big_integer const &b);

big_integer operator/(big_integer a, big_integer const &b);

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
