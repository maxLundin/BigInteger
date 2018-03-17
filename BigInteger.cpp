#include "BigInteger.h"


/*
    BigInteger::BigInteger() {
        digits.push_back(0);
        sign = 0;
    }
*/
BigInteger::BigInteger(long long x) {
    if (x < 0) {
        sign = -1;
    } else {
        sign = 1;
    }
    while (x) {
        digits.push_back(x % BASE);
        x /= BASE;
    }
}

BigInteger::BigInteger(const BigInteger &other) {
    sign = other.sign;
    for (long long digit: other.digits) {
        digits.push_back(digit);
    }
}

BigInteger::BigInteger(BigInteger &&other) {
    digits.clear();
    sign = other.sign;
    for (long long digit: other.digits) {
        digits.push_back(digit);
    }
}

BigInteger &BigInteger::operator+=(const BigInteger &other) {
    BigInteger n(*this);
    if (sign == other.sign) {
        long long propagate = 0;
        for (int i = 0; i < std::max(digits.size(), other.digits.size()); i++) {
            if (digits.size() == i) {
                digits.push_back(0);
            }
            if (other.digits.size() == i) {
                for (int j = i; j < digits.size(); j++) {
                    long long prop = propagate;
                    propagate = (digits[j] + propagate) / BASE;
                    digits[j] = (digits[j] + prop) % BASE;
                }
                break;
            }
            long long prop = propagate;
            propagate = (other.digits[i] + digits[i] + propagate) / BASE;
            digits[i] = (other.digits[i] + digits[i] + prop) % BASE;

        }
        digits.push_back(propagate);
        cutBadZero();
    } else {
        if (other.sign == -1) {
            BigInteger m(other);
            m.sign = 1;
            *this = n - m;
        } else {
            BigInteger m(*this);
            m.sign = 1;
            *this = other - m;
        }
    }
    return *this;
}


BigInteger &BigInteger::operator-=(const BigInteger &other) {
    BigInteger n(*this);
    if (sign == other.sign) {
        if (n > other) {
            int propagate = 0;
            for (int i = 0; i < std::max(digits.size(), other.digits.size()); i++) {
                if (other.digits.size() == i) {
                    for (int j = i; j < digits.size(); j++) {
                        digits[j] = (digits[j] + propagate);
                        if (digits[j] < 0) {
                            digits[j] += BASE;
                            propagate = -1;
                        } else {
                            propagate = 0;
                        }

                    }
                    break;
                }
                digits[i] = (digits[i] - other.digits[i] + propagate);
                if (digits[i] < 0) {
                    digits[i] += BASE;
                    propagate = -1;
                } else {
                    propagate = 0;
                }
            }
            cutBadZero();

        } else {
            BigInteger m = other - n;
            m.sign = this->sign * (-1);
            *this = m;
        }

    } else {
        if (other.sign == -1) {
            BigInteger m(other);
            m.sign = 1;
            *this = n + m;
        } else {
            BigInteger m(other);
            m.sign = -1;
            *this = n + m;
        }
    }
    return *this;
}


BigInteger BigInteger::operator-(const BigInteger &other) const {
    BigInteger n(*this);
    n -= other;
    return n;
}

void BigInteger::cutBadZero() {
    while ((int) digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

std::ostream &operator<<(std::ostream &stream, const BigInteger &curr) {
    if (curr.digits.empty() || (curr.digits.size() == 1 && curr.digits[0] == 0)) {
        stream << 0;
    } else {
        if (curr.sign == -1) {
            stream << "-";
        }
        stream << curr.digits[curr.digits.size() - 1];
        for (int i = (int) curr.digits.size() - 2; i > -1; i--) {
            stream << std::setw(static_cast<int>(curr.BASE_SIZE)) << std::setfill('0') << curr.digits[i];
        }
    }
    return stream;

}

std::istream &operator>>(std::istream &stream, BigInteger &curr) {
    std::string number;
    stream >> number;
    if (number[0] == '-') {
        curr.sign = -1;
        number = number.substr(1, number.size());
    } else {
        curr.sign = 1;
    }
    for (auto i = (int) number.size(); i > 0; i -= curr.BASE_SIZE) {
        if (i < curr.BASE_SIZE) {
            curr.digits.push_back(stoi(number.substr(0, static_cast<unsigned long>(i))));
        } else {
            curr.digits.push_back(stoi(number.substr(static_cast<unsigned long>(i - curr.BASE_SIZE),
                                                     static_cast<unsigned long>(curr.BASE_SIZE))));
        }
    }
    curr.cutBadZero();
    return stream;
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
    BigInteger n(*this);
    n += other;
    return n;
}

bool BigInteger::operator>(const BigInteger &other) {
    if (digits.size() > other.digits.size()) {
        return true;
    } else if (digits.size() == other.digits.size()) {
        for (auto i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] > other.digits[i]) {
                return true;
            } else if (digits[i] < other.digits[i]) {
                return false;
            }
        }
    } else {
        return false;
    }
}


BigInteger BigInteger::operator*(const BigInteger &other) const {
    BigInteger bigInt(0);
    bigInt.sign = sign * other.sign;
    long long result;
    int pointer = 0;
    int pointer_copy = pointer;
    for (int digit : digits) {
        long long propagate = 0;
        for (int j : other.digits) {
            result = (long long) digit * (long long) j;
            if (bigInt.digits.size() <= pointer) {
                bigInt.digits.push_back(0);
            }
            long long res = (bigInt.digits[pointer] + propagate + (result) % BASE);
            bigInt.digits[pointer] = static_cast<int>(res % BASE);
            propagate = result / BASE;
            propagate += (res / BASE);
            pointer += 1;
        }
        if (propagate != 0 && bigInt.digits.size() <= pointer) {
            bigInt.digits.push_back(0);
        }
        bigInt.digits[pointer] += propagate;

        pointer = (++pointer_copy);
    }
    return bigInt;
}


BigInteger BigInteger::operator<<(const short shift) {
    long long propagate = 0;
    for (unsigned int &digit : digits) {
        //std::cout << digit << std::endl;
        digit <<= shift;
        std::cout << digit << std::endl;
        digit = static_cast<unsigned int>(digit % BASE + propagate);
        //std::cout << digit << std::endl;
        propagate = digit >> (BASE - shift);
    }
    while (propagate != 0) {
        digits.push_back(static_cast<unsigned int &&>(propagate % BASE));
        propagate /= BASE;
    }
    return *this;
}


//BigInteger BigInteger::operator>>(const short shift) {
//    long long propagate = 0;
//    for (unsigned long i = digits.size(); i >-1 ; i++ ){
//        long long result = digits[i] >> shift;
//        long long res = (propagate + (result) % BASE);
//        digit = static_cast<int>(res % BASE);
//        propagate = result / BASE + res / BASE;
//    }
//    while (propagate != 0) {
//        digits.push_back(static_cast<int &&>(propagate % BASE));
//        propagate /= BASE;
//    }
//    return *this;
//}

