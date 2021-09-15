#pragma once

//#include "OptimizedArrayList.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>


struct big_integer {

    big_integer();

    big_integer(const big_integer &other) = default;

    big_integer(int a);

    big_integer(size_t a, bool);

    explicit big_integer(std::string const &str);

    ~big_integer() = default;

    big_integer(big_integer &&other) = default;

    friend std::ostream &operator<<(std::ostream &, big_integer const &);

    friend std::istream &operator>>(std::istream &, big_integer &);

    [[nodiscard]] std::string to_string() const;

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

    friend void swap(big_integer &a, big_integer &b) noexcept;

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~();

    big_integer &operator++();

    big_integer &operator--();

    big_integer &operator=(big_integer const &other) = default;

    big_integer &operator=(big_integer &&other) = default;

private:
    void cutBadZero();

    big_integer reverseIt();

    void mul_long_short(unsigned int number);

    void add_long_short(unsigned int number);

    friend std::string to_string(big_integer const &a);

    bool compare_without_sign_and_equals(const big_integer &);

    unsigned int div_long_short(unsigned int number);

    [[nodiscard]] short compare(const big_integer &other) const;

    friend void divide(big_integer &res, const big_integer &a, const big_integer &b);

    friend void sub(big_integer &a, const big_integer &b);

    std::vector<uint32_t> digits;
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

void swap(big_integer &a, big_integer &b) noexcept;

void divide(big_integer &res, const big_integer &a, const big_integer &b);
