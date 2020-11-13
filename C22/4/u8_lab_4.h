#ifndef U8_LAB_4_H
#define U8_LAB_4_H

#include <string>
#include <stack>
// #include <regex>
#include <exception>

// Task #1
/*
enum class NumberType {
    Integer,
    Hexadecimal,
    Float
};

template <typename T> T parseNumber(const std::string& input, NumberType nt) {
    try {
        std::regex pattern;
        int base = 10;
        switch (nt) {
            case NumberType::Integer:
                pattern = R"(^([\+\-]?[1-9][0-9]*)$)";
                base = 10;
                break;
            case NumberType::Hexadecimal:
                pattern = R"(^(0x[0-9|a-f|A-F]+)$)";
                base = 16;
                break;
            case NumberType::Float:
                // -2.33 или +1.23e+06 или 0.245E10
                pattern = R"(^([\+\-]?[0-9]*\.[0-9]+([e|E][\+\-]?[0-9]+)?)$)";
                break;
        }
        std::smatch matches;
        if (std::regex_search(input, matches, pattern) and matches[1].matched) {
            if (nt == NumberType::Integer or nt == NumberType::Hexadecimal) {
                return std::stoi(matches[0], 0, base);
            }
            if (nt == NumberType::Float) {
                return std::stod(matches[0]);
            }
        } else {
            throw std::runtime_error(input + " - это не число!");
        }
        return T();
    } catch (const std::exception&) {
        throw std::runtime_error(input + " - это не число!");
    }
}

bool isDecimalDigitIn(const std::string& input) {
    try {
        std::smatch matches;
        if (std::regex_search(input, matches, std::regex(R"(.*[0-9].*)"))) {
            return true;
        } else {
            return false;
        }
    } catch (const std::exception&) {
        return false;
    }
    return false;
}

std::vector<int> getAllDecimalDigits(const std::string& input) {
    std::vector<int> result;
    try {
        std::regex pattern(R"([^0-9]*([0-9]).*)");
        std::smatch matches;
        auto first{cbegin(input)};
        auto last{cend(input)};
        while (std::regex_search(first, last, matches, pattern)) {
            first = matches[1].second;
            result.push_back(std::stoi(matches[1]));
        }
    } catch (const std::exception&) {
        return result;
    }
    return result;
}
*/
// Task #2

class A {
    public :
        A(int i) : data{i} {}
        int getData() const { return data; }
    private:
        int data;
};

std::ostream& operator<<(std::ostream& os, const A& obj) {
    os << obj.getData();
    return os;
}

inline void MyPrint() {
    std::cout << std::endl;
}

template <typename T, typename ... L> void MyPrint(T first, L ... other) {
    std::cout << first << " ";
    MyPrint(other ...);
}

// Task #3

template <typename T> decltype(T() + T()) add(T a, T b) { return a + b; }

struct Multiplier {
    double operator()(double a, double b) { return a * b; }
};

class Selector {
    public:
        double max(double a, double b) { return std::max(a, b); }
};

class Calculator {
    public:
        Calculator() :
            unaryActions{
                { L'±', [](double a) { return -a; }}
            },
            binaryActions{
                { L'+', add<double> },
                { L'-', std::minus<double>{} },
                { L'*', Multiplier{} },
                { L'/', [](double a, double b) { return a / b; }},
                { L'^', static_cast<double (*)(double, double)>(std::pow) },
                { L'%', std::bind(
                    static_cast<double (*)(double, double)>(std::remainder),
                    std::placeholders::_1,
                    std::placeholders::_2
                )},
                { L'?', std::bind(
                    &Selector::max,
                    Selector{},
                    std::placeholders::_1,
                    std::placeholders::_2
                )}
            }
        {}
        double calculate(const std::wstring& input) {
            double result = 0.0;
            std::stack<double> operands;
            for (size_t i = 0; i < input.size(); ++i) {
                if (isFloatChar(input[i])) {
                    std::wstring a;
                    while (not isDelimeter(input[i]) and not isOperator(input[i])) {
                        a += input[i++];
                        if (i == input.size()) break;
                    }
                    try {
                        operands.push(stod(a));
                    } catch (...) {
                        throw std::runtime_error("Invalid argument.");
                    }
                    --i;
                } else if (isUnaryOperator(input[i])) {
                    if (operands.size() < 1) {
                        throw std::runtime_error("Insufficient arguments.");
                    }
                    double a = operands.top();
                    operands.pop();
                    result = unaryActions[input[i]](a);
                    operands.push(result);
                } else if (isBinaryOperator(input[i])) {
                    if (operands.size() < 2) {
                        throw std::runtime_error("Insufficient arguments.");
                    }
                    double b = operands.top();
                    operands.pop();
                    double a = operands.top();
                    operands.pop();
                    result = binaryActions[input[i]](a, b);
                    operands.push(result);
                }
            }
            return result;
        }
    private:
        bool isFloatChar(wchar_t ch) {
            static const std::wstring Characters{L"0123456789."};
            return Characters.find(ch) != std::wstring::npos;
        }
        bool isDelimeter(wchar_t ch) {
            static const std::wstring Characters{L" "};
            return Characters.find(ch) != std::wstring::npos;
        }
        bool isUnaryOperator(wchar_t ch) {
            static const std::wstring Characters{L"±"};
            return Characters.find(ch) != std::wstring::npos;
        }
        bool isBinaryOperator(wchar_t ch) {
            static const std::wstring Characters{L"+-*/^%?"};
            return Characters.find(ch) != std::wstring::npos;
        }
        bool isOperator(wchar_t ch) {
            return isUnaryOperator(ch) or isBinaryOperator(ch);
        }
        std::map<wchar_t, std::function<double(double)>> unaryActions;
        std::map<wchar_t, std::function<double(double, double)>> binaryActions;
};

#endif // U8_LAB_4_H
