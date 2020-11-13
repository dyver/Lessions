#include <iostream>
#include <sstream>
#include <cassert>
#include <typeinfo>

// Task #4

template <typename T> void PrintAnyCont(const T& c) {
    for (const auto& el : c) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

// Task #5

template <typename T> void NegateAll(T& c) {
    for (auto& el : c) {
        el = -el;
    }
}

inline auto operator-(const std::string& s) {
    auto res{s};
    for (auto& el : res) {
        if (std::isupper(el)) {
            el = tolower(el);
        } else {
            el = toupper(el);
        }
    }
    return s;
}

// Task #4a

template <typename T> void IncAnyCont(T& c) {
    for (auto& el : c) {
        ++el;
    }
}

// Task #5

template <typename T> void absSort(T& c) {
    std::sort(
        std::begin(c),
        std::end(c),
        [](const auto& a, const auto& b) { return std::abs(a) < std::abs(b); }
    );
}

// Task #6

template <typename T, typename U> auto SumCont(const T& t, const U& u) {
    auto s = std::max(std::size(t), std::size(u));
    using Result = std::vector<decltype(*std::begin(t) + *std::begin(u))>;
    auto action = [](auto a, auto b) { return a + b; };
    if (s == std::size(t)) {
        Result result;
        result.assign(std::cbegin(t), std::cend(t));
        transform(std::cbegin(u), std::cend(u), std::cbegin(t), std::begin(result), action);
        return result;
    } else {
        Result result;
        result.assign(std::cbegin(u), std::cend(u));
        transform(std::cbegin(t), std::cend(t), std::cbegin(u), std::begin(result), action);
        return result;
    }
}

// Task #7

template <typename T, typename U1, typename U2, typename P> void Separate(
    const T& t, U1& u1, U2& u2, P p
) {
    for (const auto& el : t) {
        if (p(el)) {
            u1.insert(std::end(u1), el);
        } else {
            u2.insert(std::end(u2), el);
        }
    }
}

// Task #9

template <typename T> class Container {
    public:
        using CT = std::vector<T>;
        Container(const std::initializer_list<T>& data, const T& low = T{}, const T& high = T{})
            : low{low}, high{high}
        {
            assert(low <= high);
            for (const auto& el : data) {
                if (find(std::begin(c), std::end(c), el) == std::end(c)) {
                    if (low == high or (el >= low and el <= high)) {
                        c.insert(std::end(c), el);
                    }
                }
            }
        }
        Container& operator=(const std::initializer_list<T>& data) {
            c.clear();
            copy_if(cbegin(data), cend(data), back_inserter(c), [this](const T& el) {
                if (find(std::begin(c), std::end(c), el) == std::end(c)) {
                    if (low == high or (el > low and el < high)) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            });
            return *this;
        }
        void remove(const std::initializer_list<T>& data) {
            for (const auto& el : data) {
                c.erase(std::remove(std::begin(c), std::end(c), el), std::end(c));
            }
        }
        void sort(bool is_ascending) {
            std::sort(
                std::begin(c), std::end(c), [is_ascending](const T& a, const T& b) {
                    return is_ascending ? a < b : b < a;
                }
            );
        }
        typename CT::iterator begin() { return std::begin(c); }
        typename CT::iterator end() { return std::end(c); }
        typename CT::const_iterator begin() const { return std::cbegin(c); }
        typename CT::const_iterator end() const { return std::cend(c); }
    private:
        CT c;
        const T& low;
        const T& high;
};

// Task #10

template <typename E> using EnumElementType = std::underlying_type_t<E>;
template <typename E> using EnumContainerType = std::map<std::string, EnumElementType<E>>;

template <typename E> struct EnumConverter {
    static EnumContainerType<E> c;
};

template <typename E> void registerEnum(std::initializer_list<std::pair<const char*, E>> data) {
    for (const auto& el : data) {
        EnumConverter<E>::c[el.first] = static_cast<EnumElementType<E>>(el.second);
    }
}

template <typename E> E stringToEnum(const std::string& name) {
    if (EnumConverter<E>::c.find(name) == EnumConverter<E>::c.end()) {
        throw std::out_of_range("Element " + name + " not found in enum " + typeid(E).name());
    }
    return static_cast<E>(EnumConverter<E>::c[name]);
}

template <typename E> auto enumToString(E e) {
    for (const auto& el : EnumConverter<E>::c) {
        if (el.second == static_cast<EnumElementType<E>>(e)) {
            return el.first;
        }
    }
    std::stringstream ss;
    ss << "Element with value " << static_cast<EnumElementType<E>>(e) << " not found in enum " << typeid(E).name();
    throw std::out_of_range(ss.str());
}

template <typename E> EnumContainerType<E> EnumConverter<E>::c;
