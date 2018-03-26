#include "BigInteger.h"

BigInteger::BigInteger(long long x) {
    if (x < 0) {
        sign = -1;
    } else {
        sign = 1;
    }
    x = std::abs(x);
    while (x > 0) {
        digits.push_back((unsigned int) (x & ((1LL << 33) - 1)));
        x >>= 32;
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
        unsigned long long propagate = 0;
        for (int i = 0; i < std::max(digits.size(), other.digits.size()); i++) {
            if (digits.size() == i) {
                digits.push_back(0);
            }
            if (other.digits.size() == i) {
                for (int j = i; j < digits.size(); j++) {
                    unsigned long long result = (unsigned long long) digits[j] + propagate;
                    propagate = (result >> 32);
                    digits[j] = (unsigned int) (result & ((1LL << 33) - 1));
                }
                break;
            }
            unsigned long long result = (unsigned long long) other.digits[i]
                                        + (unsigned long long) digits[i]
                                        + propagate;
            propagate = (result >> 32);
            digits[i] = (unsigned int) (result & ((1LL << 33) - 1));
        }
        digits.push_back((unsigned int) propagate);
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
        if (n >= other) {
            int propagate = 0;
            for (int i = 0; i < other.digits.size(); i++) {
                long long result = (long long) digits[i] - (long long) other.digits[i]
                                   + (long long) propagate;
                if (result < 0) {
                    result += ((1LL << 33));
                    digits[i] = static_cast<unsigned int>(result);
                    propagate = -1;
                } else {
                    digits[i] = static_cast<unsigned int>(result);
                    propagate = 0;
                }
            }
            if (propagate != 0) {
                digits[other.digits.size()] += propagate;
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

unsigned int BigInteger::div_long_short(unsigned int number) {
    unsigned long long propagate = 0;
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
        long long temp = (digits[i] + (propagate << 32));
        digits[i] = temp / number;
        propagate = temp % number;
    }
    cutBadZero();
    return (int) propagate;
}

void BigInteger::mul_long_short(unsigned int number) {
    unsigned long long propagate = 0;
    for (unsigned int &digit : digits) {
        unsigned long long result = (unsigned long long) digit * (unsigned long long) number + propagate;
        digit = (unsigned int) (result & ((1LL << 33) - 1));
        propagate = (result >> 32);
    }
    if (propagate) {
        digits.push_back(propagate);
    }
}

void BigInteger::add_long_short(unsigned int number) {
    unsigned long long propagate = number;
    for (unsigned int &digit : digits) {
        //std::cout << digit << std::endl;
        unsigned long long result = (unsigned long long) digit + propagate;
        digit = (result & ((1LL << 33) - 1));
        propagate = (result >> 32);
        //std::cout << result  << " " << digit << " " << propagate << std::endl;
    }
    if (propagate) {
        digits.push_back(propagate);
    }
}

std::string toString(unsigned long number) {
    std::string str = "";
    if (number == 0) {
        return "0";
    }
    while (number > 0) {
        str += ((number % 10) + '0');
        number /= 10;
    }
    //reverse(str.begin(),str.end());
    return str;
}

unsigned int toInteger(char ch) {
    return (ch - '0');
}

unsigned long long toInteger(std::string str) {
    unsigned long long answer = 0;
	for (int i = 0 ; i < str.length() ; i++){
    	answer *= 10;
		answer += toInteger(str[i]);
    }
    return answer;
}

std::ostream &operator<<(std::ostream &stream, BigInteger &curr) {
    if (curr.digits.empty() || (curr.digits.size() == 1 && curr.digits[0] == 0)) {
        stream << 0;
    } else {
        BigInteger BI(curr);
        stream << (BI.sign == 1 ? "" : "-");
        std::string outStr = "";
        while (!(BI.digits.size() == 1 && BI.digits[0] == 0)) {
            unsigned int out = BI.div_long_short(10);
            outStr += toString(out);
        }
        reverse(outStr.begin(), outStr.end());
        std::cout << outStr << std::endl;
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
    for (char i : number) {
        curr.mul_long_short(10);
        curr.add_long_short(toInteger(i));
    }
    curr.cutBadZero();
    return stream;
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
    BigInteger n(*this);
    n += other;
    return n;
}

bool BigInteger::compare_without_sign(const BigInteger &Big) {
    if (digits.size() > Big.digits.size()) {
        return true;
    } else if (digits.size() == Big.digits.size()) {
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] > Big.digits[i]) {
                return true;
            } else if (digits[i] < Big.digits[i]) {
                return false;
            }
        }
        return false;
    } else {
        return false;
    }
}

bool BigInteger::compare_without_sign_and_equals(const BigInteger &other) {
    if (digits.size() > other.digits.size()) {
        return true;
    } else if (digits.size() == other.digits.size()) {
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] < other.digits[i]) {
                return false;
            } else if (digits[i] > other.digits[i]) {
                return true;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool BigInteger::operator>(const BigInteger &other) {
    cutBadZero();
    BigInteger Big(other);
    Big.cutBadZero();

    if (sign == Big.sign) {
        bool res = compare_without_sign(other);
        return sign == 1 == res;
    } else {
        if (digits.size() == other.digits.size() && other.digits.size() == 1
            && digits[0] == 0 && other.digits[0] == 0) {
            return false;
        }
        return sign > Big.sign;
    }
}

BigInteger BigInteger::operator*(const BigInteger &other) const {
    BigInteger bigInt(0);
    bigInt.sign = sign * other.sign;
    unsigned long long result;
    int pointer = 0;
    int pointer_copy = pointer;
    for (unsigned int digit : digits) {
        unsigned long long propagate = 0;
        for (unsigned int digit1 : other.digits) {
            if (bigInt.digits.size() <= pointer) {
                bigInt.digits.push_back(0);
            }
            result = (unsigned long long) digit * (unsigned long long) digit1 + propagate +
                     (unsigned long long) bigInt.digits[pointer];
            bigInt.digits[pointer] = static_cast<unsigned int>(result & ((1LL << 33) - 1));
            propagate = result >> 32;
            pointer++;
        }
        if (bigInt.digits.size() <= pointer) {
            bigInt.digits.push_back(0);
        }
        bigInt.digits[pointer] += propagate;
        pointer = (++pointer_copy);
    }
    bigInt.cutBadZero();
    return bigInt;
}

BigInteger &BigInteger::operator*=(const BigInteger &other) {
    *this = *this * other;
    return *this;
}

bool BigInteger::operator==(const BigInteger &other) {
    if (sign != other.sign) {
        return digits.size() == other.digits.size() && other.digits.size() == 1
               && digits[0] == 0 && other.digits[0] == 0;
    }
    if (digits.size() != other.digits.size()) {
        return false;
    } else if (digits.size() == other.digits.size()) {
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }
        return true;
    }
}

bool BigInteger::operator>=(const BigInteger &other) {
    if (sign == other.sign) {
        bool res = compare_without_sign_and_equals(other);
        if (sign == 1) {
            return res;
        } else {
            return !res;
        }
    } else {
        if (digits.size() == other.digits.size() && other.digits.size() == 1
            && digits[0] == 0 && other.digits[0] == 0) {
            return true;
        } else {
            return sign > other.sign;
        }
    }
}

bool BigInteger::operator<=(const BigInteger &other) {
    BigInteger n(*this);
    return !(*this > other);
}

bool BigInteger::operator!=(const BigInteger &other) {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger &other) {
    return !(*this >= other);
}

BigInteger &BigInteger::operator<<=(short shift) {
    short shift1;
    while (shift > 0) {
        if (shift >= 16) {
            shift1 = 16;
        } else {
            shift1 = shift;
        }
        shift -= shift1;
        unsigned long long result;
        unsigned long long propagate = 0;
        for (unsigned int &digit : digits) {
            result = (((unsigned long long) (digit)) << shift1);
            digit = (unsigned int) ((result & ((1LL << 33) - 1)) + propagate);
            propagate = (result >> 32);
        }
        if (propagate != 0) {
            digits.push_back((unsigned int) (propagate));
        }
    }
    return *this;
}

BigInteger &BigInteger::operator>>=(short shift) {
    short shift1;
    while (shift > 0) {
        if (shift >= 16) {
            shift1 = 16;
        } else {
            shift1 = shift;
        }
        shift -= shift1;
        unsigned long long propagate = 0;
        unsigned long long result;
        for (int i = digits.size() - 1; i >= 0; i--) {
            //std::cout << "hui" << std::endl;
            unsigned long long prop = digits[i] & ((1 << (shift1)) - 1);
            //std::cout << prop << std::endl;
            result = (((unsigned long long) digits[i]) >> shift1);
            //std::cout << result << std::endl;
            digits[i] = (unsigned int) ((result & ((1LL << 33) - 1)) + (propagate << (32 - shift1)));
            //std::cout << digits[i] << std::endl;
            propagate = prop;
        }
    }
    cutBadZero();
    return *this;
}

BigInteger BigInteger::operator<<(short shift) {
    BigInteger n(*this);
    n <<= shift;
    return n;
}

BigInteger BigInteger::operator>>(short shift) {
    BigInteger n(*this);
    n >>= shift;
    return n;
}

BigInteger BigInteger::operator/(const BigInteger &other) const {
    BigInteger Result(0);
    BigInteger Dividend(*this);
    BigInteger shifter(1);
    Dividend.sign = 1;
    BigInteger Divider(other);
    Divider.sign = 1;
    unsigned int divLen = 0;
    while (Dividend >= Divider) {
        Divider <<= 1;
        ++divLen;
    }
    while (true) {
        while (Dividend < Divider && divLen > 0) {
            Divider >>= 1;
            --divLen;
        }

        if (divLen == 0) {
            if (Dividend >= Divider) {
                Dividend -= (Divider);
                Result += (shifter << (divLen));
            }
            return Result;
        } else {
            Dividend -= (Divider);
            Result += (shifter << (divLen));
        }
    }
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
    BigInteger Dividend(*this);
    Dividend.sign = 1;
    BigInteger Divider(other);
    Divider.sign = 1;
    unsigned int divLen = 0;
    while (Dividend >= Divider) {
        Divider <<= 1;
        ++divLen;
    }
    while (true) {
        while (Dividend < Divider && divLen > 0) {
            Divider >>= 1;
            --divLen;
        }
        if (divLen == 0) {
            if (Dividend >= Divider) {
                Dividend -= (Divider);
            }
            return Dividend;
        } else {
            Dividend -= (Divider);
        }
    }
}
