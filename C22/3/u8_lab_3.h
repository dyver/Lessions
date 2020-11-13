#ifndef U8_LAB_3_H
#define U8_LAB_3_H

#include <sstream>
#include <climits>

template <unsigned long long i> struct ConstexprChecker {
    ConstexprChecker() {} // supresses unused variable warning
};

// Task #2

constexpr int factorial(int v) {
    return v ? v * factorial(v - 1) : 1;
}

// Task #3a

constexpr int strlength(const char* p) {
    return p[0] ? strlength(p + 1) + 1 : 0;
}

constexpr long operator""_b(const char* p) {
    return p[0] ? (p[0] == '1' ? (1 << strlength(p + 1)) : 0) + operator""_b(p + 1) : 0;
}

// Task #3b

std::string operator""_toBinStr(unsigned long long v) {
    using ull = unsigned long long;
    std::string result;
    std::stringstream ss;
    ss << "0b";
    ull i = sizeof(ull) * CHAR_BIT - 1;
    do {
        ull rest = v >> i;
        bool bit = rest & 0x1;
        if (not rest) continue;
        ss << bit;
    } while (i-- != 0);
    return ss.str();
}

// Task #4a

template <typename T> class Diapason {
    public:
        constexpr Diapason(const T& low, const T& high) : low(low), high(high) {}
        constexpr T getLow() const { return low; }
        constexpr T getHigh() const { return high; }
        constexpr bool isIn(const T v) const { return v >= low and v <= high; }
        constexpr T toBounds(const T& v) const {
            return v < low ? low : (v > high ? high : v);
        }
    private:
        T low;
        T high;
};

// Task #4b

template <unsigned d = sizeof(char), typename UT = unsigned long long> class Bits {
    public:
        constexpr Bits(UT i1, UT i2, UT i3) : i1{i1}, i2{i2}, i3{i3} {}
        constexpr auto first() const { return i1; }
        constexpr auto second() const { return i1; }
        constexpr auto third() const { return i1; }
    private:
        const size_t elements = 3;
        UT i1:d;
        UT i2:d;
        UT i3:d;
};

// Task #5

template <typename C> void printContainer(const C& c) {
    for (const auto& el : c) {
        std::cout << *el << " ";
    }
    std::cout << std::endl;
}

template <typename C> void appendSuffix(C& c, const std::string& suffix) {
    for (auto& el : c) {
        *el += suffix;
    }
}

#endif // U8_LAB_3_H
