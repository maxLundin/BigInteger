#include "big_integer.h"

const uint64_t MAX_UINT32_IN_UINT64 = 4294967296u;

big_integer::big_integer() {
    sign = 1;
    digits.push_back(0);
}

big_integer::big_integer(int x) {
    uint32_t num;
    if (x < 0) {
        sign = -1;
        num = (uint32_t) (-x);
    } else {
        num = (uint32_t) x;
        sign = 1;
    }
    digits.push_back(num);
}

big_integer::big_integer(size_t x, bool flag) {
    sign = 1;
    digits.resize(x);
}

big_integer::big_integer(const big_integer &other) {
    sign = other.sign;
    for (uint32_t digit: other.digits) {
        digits.push_back(digit);
    }
}

big_integer::big_integer(big_integer &&other) noexcept {
    digits.clear();
    sign = other.sign;
    for (uint32_t digit: other.digits) {
        digits.push_back(digit);
    }
}

big_integer &big_integer::operator+=(const big_integer &b) {
    big_integer other(b);
    big_integer n(*this);
    if (sign == other.sign) {
        uint64_t propagate = 0;
        while (digits.size() < other.digits.size()) {
            digits.push_back(0);
        }
        while (digits.size() > other.digits.size()) {
            other.digits.push_back(0);
        }
        for (uint32_t i = 0; i < digits.size(); i++) {
            uint64_t result = (uint64_t) other.digits[i]
                              + (uint64_t) digits[i]
                              + propagate;
            propagate = (result >> 32);
            digits[i] = (uint32_t) (result);
        }
        digits.push_back((uint32_t) propagate);
        cutBadZero();
    } else if (other.sign == -1) {
        big_integer m(other);
        m.sign = 1;
        //std::cout << std::endl;
        //std::cout << n << m << std::endl;
        *this = n - m;
    } else {
        big_integer m(*this);
        m.sign = 1;
        *this = other - m;
    }
    return *this;
}

big_integer &big_integer::operator-=(const big_integer &other) {
    big_integer n(*this);
    if (sign == other.sign) {
        if (n.compare_without_sign_and_equals(other)) {
            uint32_t propagate = 0;
            for (size_t i = 0; i < other.digits.size(); i++) {
                int64_t result = (int64_t) digits[i] - (int64_t) other.digits[i]
                                 - (int64_t) propagate;
                if (result < 0) {
                    result += (MAX_UINT32_IN_UINT64);
                    digits[i] = (uint32_t) (result);
                    propagate = 1;
                } else {
                    digits[i] = (uint32_t) (result);
                    propagate = 0;
                }
            }
            if (propagate) {
                digits[other.digits.size()] -= propagate;
            }
        } else {
            big_integer m = other - n;
            m.sign = sign * (-1);
            *this = m;
        }
    } else if (other.sign == -1) {
        big_integer m(other);
        m.sign = 1;
        *this = n + m;
    } else {
        big_integer m(other);
        m.sign = -1;
        *this = n + m;
    }
    cutBadZero();

    return *this;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

void big_integer::cutBadZero() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

uint32_t big_integer::div_long_short(uint32_t number) {
    uint64_t propagate = 0;
    for (size_t i = (digits.size()); i--;) {
        uint64_t temp = (digits[i] + (propagate << 32));
        digits[i] = (uint32_t) (temp / number);
        propagate = temp % number;
    }
    cutBadZero();
    return (uint32_t) propagate;
}

void big_integer::mul_long_short(uint32_t number) {
    uint32_t propagate = 0;
    for (uint32_t &digit : digits) {
        uint64_t result = (uint64_t) digit * (uint64_t) number + propagate;
        digit = (uint32_t) (result);
        propagate = (uint32_t) (result >> 32);
    }
    if (propagate) {
        digits.push_back(propagate);
    }
}

void big_integer::add_long_short(uint32_t number) {
    uint32_t propagate = number;
    for (uint32_t &digit : digits) {
        uint64_t result = (uint64_t) digit + propagate;
        digit = (uint32_t) (result);
        propagate = (uint32_t) (result >> 32);
        if (propagate == 0) {
            break;
        }
    }
    if (propagate) {
        digits.push_back((uint32_t) propagate);
    }
}

std::string toString(uint64_t number) {
    std::string str;
    if (number == 0) {
        return "0";
    }
    while (number > 0) {
        str += std::to_string((number % 10) + (uint32_t) '0');
        number /= 10;
    }
    return str;
}

uint32_t toInteger(char ch) {
    return (uint32_t) (ch - '0');
}

std::string big_integer::to_string() {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
        return "0";
    } else {
        big_integer BI(*this);
        std::string outStr;

        while (!(BI.digits.size() == 1 && BI.digits[0] == 0)) {
            uint32_t out = BI.div_long_short(10);
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
    size_t start = 0;
    if (number[0] == '-') {
        sign = -1;
        start = 1;
    } else {
        sign = 1;
    }
    for (size_t i = start; i < number.size(); i++) {
        mul_long_short(10);
        add_long_short(toInteger(number[i]));
    }
    cutBadZero();
}

void swap(big_integer &a, big_integer &b) {
    using std::swap;
    swap(a.sign, b.sign);
    swap(a.digits, b.digits);
}

bool big_integer::compare_without_sign_and_equals(const big_integer &other) {
    if (digits.size() > other.digits.size()) {
        return true;
    }
    if (digits.size() == other.digits.size()) {
        for (size_t i = digits.size(); i--;) {
            int64_t diff = (int64_t) digits[i] - (int64_t) other.digits[i];
            if (diff > 0) {
                return true;
            }
            if (diff < 0) {
                return false;
            }
        }
        return true;
    }
    return false;
}

big_integer operator*(big_integer const &a, big_integer const &other) {
    if (other.digits.size() == 1) {
        big_integer bigInt(a);
        bigInt.mul_long_short(other.digits[0]);
        bigInt.sign *= other.sign;
        return bigInt;
    }

    big_integer bigInt(0);
    bigInt.sign = a.sign * other.sign;

    uint64_t result;
    uint32_t pointer = 0;
    uint32_t pointer_copy = pointer;
    bigInt.digits.resize(a.digits.size() + other.digits.size() + 1);
    for (uint32_t digit : a.digits) {
        uint64_t propagate = 0;
        for (uint32_t digit1 : other.digits) {
            result = (uint64_t) digit * (uint64_t) digit1 + propagate +
                     (uint64_t) bigInt.digits[pointer];
            bigInt.digits[pointer] = (uint32_t) result;
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
    big_integer result = (*this * other);
    swap(result, *this);
    return *this;
}

short big_integer::compare(const big_integer &other) const {
    if (digits.size() > other.digits.size()) {
        return 1;
    } else if (digits.size() == other.digits.size()) {
        for (size_t i = digits.size(); i--;) {
            if (digits[i] == other.digits[i]) {
                continue;
            }
            if (digits[i] < other.digits[i]) {
                return -1;
            } else {
                return 1;
            }
        }
        return 0;
    } else {
        return 0;
    }
}

bool operator>(const big_integer &a, const big_integer &other) {
    return (a.compare(other) == 1);
}

bool operator==(const big_integer &a, const big_integer &other) {
    return (a.compare(other) == 0);
}

bool operator>=(const big_integer &a, const big_integer &other) {
    return (a.compare(other) >= 0);
}

bool operator<=(const big_integer &a, const big_integer &other) {
    return (a.compare(other) <= 0);
}

bool operator!=(const big_integer &a, const big_integer &other) {
    return (a.compare(other) != 0);
}

bool operator<(const big_integer &a, const big_integer &other) {
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
        uint64_t result;
        uint64_t propagate = 0;
        for (uint32_t &digit : digits) {
            result = (((uint64_t) (digit)) << shift1);
            digit = (uint32_t) ((result & ((1LL << 33) - 1)) + propagate);
            propagate = (result >> 32);
        }
        if (propagate != 0) {
            digits.push_back((uint32_t) (propagate));
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
        uint64_t propagate = 0;
        uint64_t result;
        for (size_t i = digits.size(); i--;) {
            uint64_t prop = digits[i] & ((1 << (shift1)) - 1);
            result = (((uint64_t) digits[i]) >> shift1);
            digits[i] = (uint32_t) ((result & ((1LL << 33) - 1)) + (propagate << (32 - shift1)));
            propagate = prop;
        }
    }
    cutBadZero();
    return *this;
}

void big_integer::sub_equal(big_integer &a, const big_integer &b) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b.digits.size() || carry; ++i) {
        uint64_t tmp = (uint64_t) carry + (i < b.digits.size() ? b.digits[i] : 0);
        if (a.digits[i] < tmp) {
            a.digits[i] = (uint32_t) (MAX_UINT32_IN_UINT64 - tmp + a.digits[i]);
            carry = 1;
        } else {
            carry = 0;
            a.digits[i] -= tmp;
        }
    }
}

void big_integer::divide(big_integer &res, big_integer const &a, big_integer const &b) {
    if (b.digits.size() == 1) {
        res = a;
        res.div_long_short(b.digits[0]);
        return;
    }

    short neg = a.compare(b);

    res.digits.clear();

    if (neg < 0) {
        res.digits.push_back(0);
        return;
    }
    if (neg == 0) {
        res.digits.push_back(1);
        return;
    }

    auto d = (uint32_t) (MAX_UINT32_IN_UINT64 / (b.digits.back() + 1));
    big_integer u(a), v(b);
    u.mul_long_short(d);
    v.mul_long_short(d);
    u.cutBadZero();
    v.cutBadZero();
    size_t n = u.digits.size(), m = v.digits.size(), len = n - m + 1;
    res.digits.resize(len);
    big_integer dividend(m + 1, true), divider;

    for (size_t i = 0; i < m; i++) {
        dividend.digits[i] = u.digits[n + i - m];
    }
    dividend.digits[m] = n < u.digits.size() ? u.digits[n] : 0;
    for (size_t i = 0; i < len; i++) {
        dividend.digits[0] = u.digits[n - m - i];
        size_t cur_pos = len - 1 - i;

        uint32_t tmp = (uint32_t) std::min(
                ((uint64_t) (m < dividend.digits.size() ? dividend.digits[m] : 0) * MAX_UINT32_IN_UINT64 +
                            (m - 1 < dividend.digits.size() ? dividend.digits[m - 1] : 0)) / v.digits.back(),
                MAX_UINT32_IN_UINT64 - 1);


        divider = v;
        divider.mul_long_short(tmp);

        while (dividend < divider) {
            divider -= v;
            tmp--;
        }
        sub_equal(dividend, divider);
        for (size_t j = m; j > 0; j--) {
            dividend.digits[j] = dividend.digits[j - 1];
        }
        res.digits[cur_pos] = tmp;
    }
    res.cutBadZero();
}

big_integer operator/(big_integer const &Dividend, big_integer const &other) {
    if (other.digits.size() == 1) {
        big_integer temp(Dividend);
        temp.div_long_short(other.digits[0]);
        temp.sign *= other.sign;
        return temp;
    }
    big_integer ans;
    ans.divide(ans, Dividend, other);
    ans.sign = Dividend.sign * other.sign;
    return ans;
}

big_integer operator%(big_integer Dividend, big_integer const &other) {
    Dividend = Dividend - (Dividend / other) * other;
    return Dividend;
}

big_integer &big_integer::operator/=(big_integer const &other) {
    big_integer result = (*this / other);
    swap(result, *this);
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &other) {
    big_integer result = (*this % other);
    swap(result, *this);
    return *this;
}

big_integer operator&(big_integer a, big_integer const &b) {
    if (b.sign < 0) {
        big_integer d(b);
        d = d.reverseIt();
        d = d - 1;
        a &= d;
        if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a &= b;
    if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
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
        if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a |= b;
    if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
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
        if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
            a = a.reverseIt();
            a += 1;
            a.sign = -1;
        }
        return a;
    }
    a ^= b;
    if (a.digits[a.digits.size() - 1] >= (1u << 31)) {
        a = a.reverseIt();
        a += 1;
        a.sign = -1;
    }
    return a;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    uint32_t min = (uint32_t) std::min(digits.size(), rhs.digits.size());
    for (uint32_t i = 0; i < min; i++) {
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
    for (uint32_t i = 0; i < max; i++) {
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
    for (uint32_t i = 0; i < max; i++) {
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
    for (uint32_t &digit : digits) {
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
            uint32_t out = BI.div_long_short(10);
            outStr += std::to_string(out);
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

big_integer &big_integer::operator=(big_integer const &other) = default;
