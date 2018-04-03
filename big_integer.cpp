#include <gmp.h>
#include "big_integer.h"

big_integer::big_integer() {
    sign = 1;
    digits.push_back(0);
}

big_integer::big_integer(int x) {
    if (x < 0) {
        sign = -1;
    } else {
        sign = 1;
    }
    x = std::abs(x);
    digits.push_back((unsigned int) (x));

}

big_integer::big_integer(const big_integer &other) {
    sign = other.sign;
    for (long long digit: other.digits) {
        digits.push_back((unsigned int) (digit));
    }
}

big_integer::big_integer(big_integer &&other) {
    digits.clear();
    sign = other.sign;
    for (long long digit: other.digits) {
        digits.push_back((unsigned int) digit);
    }
}

big_integer &big_integer::operator+=(const big_integer &other) {
    big_integer n(*this);
    if (sign == other.sign) {
        unsigned long long propagate = 0;
        for (unsigned int i = 0; i < std::max(digits.size(), other.digits.size()); i++) {
            if (digits.size() == i) {
                digits.push_back(0);
            }
            if (other.digits.size() == i) {
                for (unsigned int j = i; j < digits.size(); j++) {
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
            big_integer m(other);
            m.sign = 1;
            *this = n - m;
        } else {
            big_integer m(*this);
            m.sign = 1;
            *this = other - m;
        }
    }
    return *this;
}

big_integer &big_integer::operator-=(const big_integer &other) {
    big_integer n(*this);
    //big_integer m(other);
    //std::cout << n  << m << std::endl;
    if (sign == other.sign) {
        if (n.compare_without_sign_and_equals(other)) {
            int propagate = 0;
            for (int i = 0; i < (int) other.digits.size(); i++) {
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
            big_integer m = other - n;
            m.sign = this->sign * (-1);
            *this = m;
        }
    } else {
        if (other.sign == -1) {
            big_integer m(other);
            m.sign = 1;
            *this = n + m;
        } else {
            big_integer m(other);
            m.sign = -1;
            *this = n + m;
        }
    }
    return *this;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

void big_integer::cutBadZero() {
    while ((int) digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

unsigned int big_integer::div_long_short(unsigned int number) {
    unsigned long long propagate = 0;
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
        unsigned long long temp = (digits[i] + (propagate << 32));
        digits[i] = temp / number;
        propagate = temp % number;
    }
    cutBadZero();
    return (int) propagate;
}

void big_integer::mul_long_short(unsigned int number) {
    unsigned long long propagate = 0;
    for (unsigned int &digit : digits) {
        unsigned long long result = (unsigned long long) digit * (unsigned long long) number + propagate;
        digit = (unsigned int) (result & ((1LL << 33) - 1));
        propagate = (result >> 32);
    }
    if (propagate) {
        digits.push_back((unsigned int) propagate);
    }
}

void big_integer::add_long_short(unsigned int number) {
    unsigned long long propagate = number;
    for (unsigned int &digit : digits) {
        //std::cout << digit << std::endl;
        unsigned long long result = (unsigned long long) digit + propagate;
        digit = (unsigned int) (result & ((1LL << 33) - 1));
        propagate = (result >> 32);
        if (propagate == 0) {
            break;
        }
        //std::cout << result  << " " << digit << " " << propagate << std::endl;
    }
    if (propagate) {
        digits.push_back((unsigned int) propagate);
    }
}

std::string toString(unsigned long number) {
    std::string str;
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
    return (unsigned int) (ch - '0');
}

std::string big_integer::to_string() {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
        return "0";
    } else {
        big_integer BI(*this);
        std::string outStr;

        while (!(BI.digits.size() == 1 && BI.digits[0] == 0)) {
            unsigned int out = BI.div_long_short(10);
            outStr += toString(out);
        }
        outStr += (BI.sign == 1 ? "" : "-");
        reverse(outStr.begin(), outStr.end());
        return outStr;
    }
}

std::ostream &operator<<(std::ostream &stream, big_integer &curr) {
    stream << curr.to_string() << std::endl;
    return stream;

}

std::istream &operator>>(std::istream &stream, big_integer &curr) {
    std::string number;
    stream >> number;
    curr = big_integer(number);
    return stream;
}

big_integer::big_integer(std::string const &number) {
    int start = 0;
    if (number[0] == '-') {
        sign = -1;
        start = 1;
    } else {
        sign = 1;
    }
    for (int i = start; i < number.size(); i++) {
        mul_long_short(10);
        add_long_short(toInteger(number[i]));
    }
    cutBadZero();
}

bool big_integer::compare_without_sign(const big_integer &Big) {
    if (digits.size() > Big.digits.size()) {
        return true;
    } else if (digits.size() == Big.digits.size()) {
        for (int i = (int) digits.size() - 1; i >= 0; i--) {
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

bool big_integer::compare_without_sign_and_equals(const big_integer &other) {
    if (digits.size() > other.digits.size()) {
        return true;
    } else if (digits.size() == other.digits.size()) {
        for (int i = (int) digits.size() - 1; i >= 0; i--) {
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

bool operator>(big_integer const &a, big_integer const &other) {
    big_integer BI(a);
    BI.cutBadZero();
    big_integer Big(other);
    Big.cutBadZero();

    if (BI.sign == Big.sign) {
        bool res = BI.compare_without_sign(other);
        if (BI.sign == 1) {
            return res;
        } else {
            return !res;
        }
    } else {
        if (BI.digits.size() == other.digits.size() && other.digits.size() == 1
            && BI.digits[0] == 0 && other.digits[0] == 0) {
            return false;
        }
        return BI.sign > Big.sign;
    }
}

big_integer operator*(big_integer a, big_integer const &other) {
    big_integer bigInt(0);
    bigInt.sign = a.sign * other.sign;
    unsigned long long result;
    unsigned int pointer = 0;
    unsigned int pointer_copy = pointer;
    for (unsigned int digit : a.digits) {
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

big_integer &big_integer::operator*=(const big_integer &other) {
    *this = *this * other;
    return *this;
}

bool operator==(big_integer const &a, big_integer const &other) {
    if (a.sign != other.sign) {
        return a.digits.size() == other.digits.size() && other.digits.size() == 1
               && a.digits[0] == 0 && other.digits[0] == 0;
    }
    if (a.digits.size() != other.digits.size()) {
        return false;
    } else {
        for (int i = (int) a.digits.size() - 1; i >= 0; i--) {
            if (a.digits[i] != other.digits[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator>=(big_integer const &a, big_integer const &other) {
    big_integer BI(a);
    if (BI.sign == other.sign) {
        bool res = BI.compare_without_sign_and_equals(other);
        if (BI.sign == 1) {
            return res;
        } else {
            return !res;
        }
    } else {
        if (BI.digits.size() == other.digits.size() && other.digits.size() == 1
            && BI.digits[0] == 0 && other.digits[0] == 0) {
            return true;
        } else {
            return BI.sign > other.sign;
        }
    }
}

bool operator<=(big_integer const &a, big_integer const &other) {
    return !(a > other);
}

bool operator!=(big_integer const &a, big_integer const &other) {
    return !(a == other);
}

bool operator<(big_integer const &a, big_integer const &other) {
    return !(a >= other);
}

big_integer &big_integer::operator<<=(short shift) {
    if (sign < 0) {
        *this = ~*this;
        *this = (*this <<= shift) += 1;
        sign = -1;
        return *this;
    }
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

big_integer &big_integer::operator>>=(short shift) {
    if (sign < 0) {
        *this = ~*this;
        *this = (*this >>= shift) += 1;
        sign = -1;
        return *this;
    }
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
        for (int i = (int) digits.size() - 1; i >= 0; i--) {
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

//big_integer operator/(big_integer Dividend, big_integer const &other) {
//    int sign = Dividend.sign;
//    big_integer Divider(other);
//    Divider.sign = 1;
//    Dividend.sign = 1;
//    int64_t bitNumber = (Dividend.digits.size()) * 32 - 1;
//    big_integer Number(0);
//    uint64_t numLen = 0;
//    uint64_t dividerLen = (Divider.digits.size() - 1) * 32;
//    uint64_t i = 0;
//    while ((Divider.digits[Divider.digits.size() - 1] >> i) != 0) {
//        //int h = Divider.digits[Divider.digits.size() - 1] >> i;
//        //std::cout << h << std::endl;
//        i++;
//    }
//
//    dividerLen += i;
//    std::cout << dividerLen << std::endl;
//    big_integer answer(0);
//    int shift;
//    while (true) {
//        shift = 0;
//        if ((bitNumber - (dividerLen - numLen)) >= 0) {
//            Number <<= (dividerLen - numLen);
//            uint64_t uk2 = dividerLen - numLen;
//            uint64_t end1 = (bitNumber - (dividerLen - numLen));
//            uint64_t end2 = 0;
//            while (uk2 != 0) {
//                uint64_t razdel1 = ((bitNumber >> 5) << 5);
//                uint64_t razdel2 = ((uk2 >> 5) << 5);
//                uint64_t shiftLen = std::min(bitNumber - std::max(razdel1, end1) - 1,
//                                             uk2 - std::max(razdel2, end2) - 1);
//
//            }
//        }
//        while (numLen < dividerLen && bitNumber >= 0) {
//            //bit = static_cast<uint8_t> ((Dividend.digits[bitNumber >> 5] >> (bitNumber & ((1 << 6) - 1))) & 1);
//            Number <<= 1;
//            Number.add_long_short(bit);
//            ++shift;
//            --bitNumber;
//        }
//        answer <<= shift;
//        if (Number >= Divider) {
//            Number -= Divider;
//            Number.cutBadZero();
//            answer.add_long_short(1);
//        }
//        if (bitNumber < 0) {
//            answer.sign = sign * other.sign;
//            return answer;
//        }
//    }
//}

big_integer operator/(big_integer Dividend, big_integer const &other) {
    int sign = Dividend.sign;
    big_integer Divider(other);
    Divider.sign = 1;
    Dividend.sign = 1;
    int64_t bitNumber = (Dividend.digits.size()) * 32 - 1;
    big_integer Number(0);
    uint64_t numLen = 0;
    uint64_t dividerLen = Divider.digits.size() * 32;
//    int i = 0;
//    while ((Divider.digits[Divider.digits.size() - 1] >> i) != 0) {
//        i++;
//    }
//    dividerLen+=i;
    uint8_t bit;
    big_integer answer(0);
    int shift;
    while (true) {
        shift = 0;
        while (Number < Divider && bitNumber >= 0) {
            bit = static_cast<uint8_t> ((Dividend.digits[bitNumber >> 5] >> (bitNumber & ((1 << 6) - 1))) & 1);
            Number <<= 1;
            Number.add_long_short(bit);
            ++shift;
            --bitNumber;
        }
        answer <<= shift;
        if (Number >= Divider) {
            Number -= Divider;
            Number.cutBadZero();
            answer.add_long_short(1);
        }
        if (bitNumber < 0) {
            answer.sign = sign * other.sign;
            return answer;
        }
    }
}

//big_integer operator/(big_integer Dividend, big_integer const &other) {
//    big_integer Result(0);
//    for (size_t i = 1; i < Dividend.digits.size(); i++) {
//        Result.digits.push_back(0);
//    }
//    int sign = Dividend.sign;
//    Dividend.sign = 1;
//    big_integer Divider(other);
//    Divider.sign = 1;
//    unsigned int divLen = 0;
//    while (Dividend >= Divider) {
//        Divider <<= 16;
//        divLen += 16;
//    }
//    while (true) {
//        while (Dividend < Divider && divLen > 0) {
//            Divider >>= 1;
//            --divLen;
//        }
//        if (divLen == 0) {
//            if (Dividend >= Divider) {
//                Dividend -= (Divider);
//                Result.digits[divLen >> 5] += (1 << (divLen & ((1 << 6) - 1)));
//            }
//            Result.sign = sign * other.sign;
//            Result.cutBadZero();
//            return Result;
//        } else {
//            Dividend -= (Divider);
//            Result.digits[divLen >> 5] += (1 << (divLen & ((1 << 6) - 1)));
//        }
//    }
//}

big_integer operator%(big_integer Dividend, big_integer const &other) {
    int sign = Dividend.sign;
    Dividend.sign = 1;
    big_integer Divider(other);
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
            Dividend.sign = sign;
            return Dividend;
        } else {
            Dividend -= (Divider);
        }
    }
}

big_integer &big_integer::operator/=(big_integer const &other) {
    *this = *this / other;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this % rhs;
    return *this;
}

big_integer operator&(big_integer a, big_integer const &b) {
    if (b.sign < 0) {
        big_integer d(b);
        d = d.reverseIt();
        d = d - 1;
        a &= d;
        if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a &= b;
    if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
        a = a.reverseIt();
        a += 1;
        a.sign = -1;
    }
    return a;
}

big_integer operator|(big_integer a, big_integer const &b) {
    if (b.sign < 0) {
        big_integer d(b);
        d = d.reverseIt();
        d = d - 1;
        a |= d;
        if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a |= b;
    if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
        a = a.reverseIt();
        a += 1;
        a.sign = -1;
    }
    return a;
}

big_integer operator^(big_integer a, big_integer const &b) {
    if (b.sign < 0) {
        big_integer d(b);
        d = d.reverseIt();
        d = d - 1;
        a ^= d;
        if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a ^= b;
    if (a.digits[a.digits.size() - 1] >= (1 << 31)) {
        a = a.reverseIt();
        a += 1;
        a.sign = -1;
    }
    return a;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    uint32_t min = (uint32_t) std::min(digits.size(), rhs.digits.size());
    for (unsigned int i = 0; i < min; i++) {
        digits[i] &= rhs.digits[i];
    }
    uint32_t size = (uint32_t) digits.size() - min;
    for (uint32_t j = 0; j < size; j++) {
        digits.pop_back();
    }
    cutBadZero();
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    uint32_t max = (uint32_t) std::max(digits.size(), rhs.digits.size());
    for (unsigned int i = 0; i < max; i++) {
        if (digits.size() <= i) {
            digits.push_back(0);
        }
        digits[i] |= rhs.digits[i];
    }
    cutBadZero();
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    uint32_t max = (uint32_t) std::max(digits.size(), rhs.digits.size());
    for (unsigned int i = 0; i < max; i++) {
        if (digits.size() <= i) {
            digits.push_back(0);
        }
        digits[i] ^= rhs.digits[i];
    }
    cutBadZero();
    return *this;
}

big_integer big_integer::operator-() const {
    return 0 - *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::reverseIt() {
    for (unsigned int &digit : digits) {
        digit = ~digit;
    }
    return *this;
}

big_integer big_integer::operator~() {
    return -*this - 1;
}

big_integer &big_integer::operator++() {
    *this = *this + 1;
    return *this;
}

big_integer &big_integer::operator--() {
    *this = *this - 1;
    return *this;
}

std::string to_string(big_integer const &a) {
    if (a.digits.empty() || (a.digits.size() == 1 && a.digits[0] == 0)) {
        return "0";
    } else {
        big_integer BI(a);
        std::string outStr;

        while (!(BI.digits.size() == 1 && BI.digits[0] == 0)) {
            unsigned int out = BI.div_long_short(10);
            outStr += toString(out);
        }
        outStr += (BI.sign == 1 ? "" : "-");
        reverse(outStr.begin(), outStr.end());
        return outStr;
    }
}

big_integer operator<<(big_integer a, int b) {
    if (a.sign < 0) {
        a = ~a;
        a = (a <<= b) += 1;
        a.sign = -1;
        return a;
    }
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    if (a.sign < 0) {
        a = ~a;
        a = (a >>= b) += 1;
        a.sign = -1;
        return a;
    }
    return a >>= b;
}

big_integer &big_integer::operator=(big_integer const &other) {
    sign = other.sign;
    digits = other.digits;
    return *this;
}

