#define nop ("nop");

//Практика 1:
// enum class
// initializer_list и универсальные списки инициализации
// auto
// decltype
// lambda

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <string>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cassert>
#include <typeinfo>

using namespace std;

template <typename T> void PrintAnyCont(const T& c) {
    for (const auto& e : c) {
        cout << e << " ";
    }
    cout << endl;
}

template <typename T> void NegateAll(T& c) {
    for (auto& e : c) {
        e = (-e);
    }
}
/*
template <> void NegateAll(list<string>& c) {
    for (auto& e : c) {
        transform(begin(e), end(e), begin(e), [](char c) { return toupper(c) == c ? tolower(c) : toupper(c); });
    }
}
*/
namespace std {
    std::string operator-(std::string& e) {
        transform(begin(e), end(e), begin(e), [](char c) { return toupper(c) == c ? tolower(c) : toupper(c); });
        return e;
    }
}

template <typename T> void absSort(T& c) {
    sort(begin(c), end(c), [](const auto& a, const auto& b) { return abs(a) < abs(b); });
}

template <typename T, typename V> auto SumCont(const T& c1, const V& c2) {
    vector<decltype((*cbegin(c1)) + (*cbegin(c2)))> res(size(c1) > size(c2) ? size(c1) : size(c2));
    copy(cbegin(c1), cend(c1), begin(res));
    transform(
        begin(c2), end(c2), begin(res), begin(res),
        [](const auto& e1, const auto& e2) { return e1 + e2; }
    );
    return res;
}

template <typename T, typename U, typename V, typename C>
void Separate(const T& f, U& c1, V& c2, C to_first) {
    c1.clear();
    c2.clear();
    for (const auto& el : f) {
        if (to_first(el)) {
            c1.insert(end(c1), el);
        } else {
            c2.insert(end(c2), el);
        }
    }
}

// Task #10

template <typename E> using EnumContainerType = std::map<std::string, E>;

template <typename E> struct EnumConverter {
    static EnumContainerType<E> c;
};

template <typename E> void registerEnum(std::initializer_list<std::pair<const char*, E>> data) {
    for (const auto& el : data) {
        EnumConverter<E>::c[el.first] = el.second;
    }
}

template <typename E> E stringToEnum(const std::string& name) {
    if (EnumConverter<E>::c.find(name) == EnumConverter<E>::c.end()) {
        throw std::out_of_range("Element " + name + " not found in enum " + typeid(E).name());
    }
    return EnumConverter<E>::c[name];
}

template <typename E> auto enumToString(E e) {
    for (const auto& el : EnumConverter<E>::c) {
        if (el.second == e) {
            return el.first;
        }
    }
    std::stringstream ss;
    ss << "Element with value " << e
        << " not found in enum " << typeid(E).name();
    throw std::out_of_range(ss.str());
}

template <typename E> EnumContainerType<E> EnumConverter<E>::c;

void task1() {
    //Задание 1.
    enum class months : char { January, February, March/*...*/ };
    enum class weekDays { Monday, Tuesday /*...*/ };

    months m = months::January;
    (void)m;
    //а) уберите проблемы (это значит, что КОМПИЛЯТОР не
    //должен позволять программисту осуществлять опасные
    //сравнения!)

//        if (m == weekDays::Monday) { /*...*/ }

    //б) оптимизируйте использование памяти
    months year[] = { months::January, months::February, months::March };
    size_t n = sizeof(year); //???
    cout << "Size of year[] = " << n << endl;

    __asm nop
}

void task2() {
    //Задание 2. Создайте и с помощью списка инициализации
    //заполните значениями вектор с элементами - string
    //С помощью range-based for преобразуйте все буквы в
    //верхний регистр а остальные символы должны остаться
    //неизменными
    vector<string> v{ "a", "B", "1", "9", "42" };
    for (auto& e : v) {
        transform(begin(e), end(e), begin(e), [](char c) { return toupper(c); });
    }
    for (const auto& e : v) {
        cout << e << " ";
    }
    cout << endl;

    __asm nop
}

void task3() {
    ///Задание 3. Создайте и заполните значениями
    //map таким образом, чтобы он содержал в качестве ключа
    //букву, а в качестве значения отсортированную по
    //алфавиту совокупность слов (string), начинающихся с
    //этой буквы

    //3а. Проинициализируйте map "вручную" посредством списка инициализации
    //С помощью range-based for и structured binding
    //распечатайте содержимое, например: A: any, apple, away
    {
        map<char, set<string>> m{
            {'a', {"apple", "any", "angry"} },
            {'z', {"zombie", "zero", "zag"} }
        };
        for (const auto& [k, v] : m) {
            cout << k << "=";
            for (const auto& e : v) {
                cout << e << " ";
            }
            cout << endl;
        }
        cout << endl;

        __asm nop
    }

    //3б. Создайте пустой map и используйте заданный массив
    //для выполнения задания.
    //С помощью range-based for и structured binding
    //распечатайте содержимое, например: A: any, apple, away
    {
        //дано (например):
        const char* s[] = { "yong", "away", "bar", "any", "son", "apple" };
        map<char, set<string>> m;
        for (auto& e : s) {
            m[e[0]].insert(e);
        }
        for (const auto& [k, v] : m) {
            cout << k << "=";
            for (const auto& e : v) {
                cout << e << " ";
            }
            cout << endl;
        }
        cout << endl;


        __asm nop
    }
}

void task4() {
    //Задание 4. создать функцию для вывода на печать
    //элементов последовательностей, заданных ниже:
    std::vector<double> vd = { 1.1,2.2,3.3 };
    PrintAnyCont(vd);

    std::string s("abc");
    PrintAnyCont(s);

    int ar[] = { 1, 2, 3 };
    PrintAnyCont(ar);

    std::initializer_list<int> il{ 3,4,5 };
    PrintAnyCont(il);

    __asm nop
}

void task5() {
    ///Задание 5.
    //Cоздать функцию для "отрицания" значений, например:
    //было: {1, -2, 5}, стало: {-1, 2, -5})
    //изменение объектов типа std::string может выглядеть "aBc1" -> "AbC1"
    //элементов последовательностей, заданных ниже:
    std::vector<double> vd{ 1.1,2.2,3.3 };
    NegateAll(vd);
    PrintAnyCont(vd);

    std::list<std::string> ls{ "aBc", "Qwerty", "n12" };
    NegateAll(ls);
    PrintAnyCont(ls);

    int ar[]{ 1, 2, 3 };
    NegateAll(ar);
    PrintAnyCont(ar);

    __asm nop
}

void task6() {
    //Задание 6. Реализовать функцию сортировки по модулю
    //элементов последовательностей, заданных ниже
    //Собственно для сортировки можно использовать обобщенный
    //алгоритм sort(), а для задания условия - лямбда-функцию
    std::vector<double> vd = { -3.3,  2.2, -1.1 };
    absSort(vd);
    PrintAnyCont(vd);

    int ar[] = { -3, 2, -1 };
    absSort(ar);
    PrintAnyCont(ar);

    __asm nop
}

void task7() {
    //Задание 7.
    //Напишите функцию, которая будет формировать и
    //возвращать вектор, каждый элемент
    //которого является суммой элементов двух
    //последовательностей РАЗНОЙ длины
    //и с элементами РАЗНОГО типа.

    //Подсказка 1: так как последовательности могут быть
    //разной длины, логично сделать размер результирующего
    //вектора максимальным из двух

    //Подсказка 2: подумайте о возможности использования
    //алгоритмов copy() и transform(), в котором
    //трансформирующее действие удобно в свою очередь
    //задавать лямбда-функцией

    //например:
    std::vector<int> v{ 1,2,3,4 };
    std::list<double> l{ 1.1, 2.2, 3.3, 4.4, 5.5 };
    auto res1 = SumCont(v, l);
    PrintAnyCont(res1);

    std::list<int> ll{ 1, 2, 3, 4, 5, 6, 7, 8 };
    double ar[] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
    auto res2 = SumCont(ar, ll);
    PrintAnyCont(res2);

    std::set<std::string> s{ "abc", "qwerty", "my"};
    std::deque<const char*> d{ "111", "22" };
    auto res3 = SumCont(s, d);
    PrintAnyCont(res3);

    __asm nop
}

void task8() {
    //Задание 8. Реализуйте функцию, которая принимает следующие параметры:
    //сформированную последовательность любого типа с элементами любого типа,
    //два (пустых) контейнера любого типа из vector, list, deque, set
    //с элементами того же типа, что и у сформированной последовательности

    //Функция должна "разложить" значения заданной последовательности в два пустых контейнера
    //согласно заданному условию. Условие задать лябда-функцией
    //Исходная последовательность при этом не меняется
    //Например:
    std::vector<int> v{ 1,2,3,4,5 };
    std::list<int> l; //сюда четные
    std::set<int> s; //сюда четные
    std::deque<int> d; //а сюда нечетные
    Separate(v, l, d, [](const auto& el) { return not (el % 2); } );
    PrintAnyCont(l);
    PrintAnyCont(d);
    Separate(v, s, d, [](const auto& el) { return not (el % 2); } );
    PrintAnyCont(s);
    PrintAnyCont(d);

    __asm nop
}

void task9() {
    //Задание 9. C помощью алгоритма for_each()!!!
    //(а не count_if()) посчитать сколько букв в верхнем
    //регистре.
    //  Использовать лямбда функцию
    char s[] = "Hello World!";
    int uppers = 0;
    for_each(
        cbegin(s), cend(s),
        [&uppers](const auto& el) {
            if (tolower(el) != el) ++uppers;
        }
    );
    cout << "Uppers=" << uppers << endl;

    __asm nop
}

void task10() {
    //Задание 10. Реализовать конвертацию enum в строковое представление  - enumToString
    // и наоборот - stringToEnum

    //Подсказки:
    //***********
    //1. Соответствующие именованным константам строки все равно нужно где-то хранить =>
    //1.1 Именованные константы в перечислении должны быть уникальными => соответствующие строки
    //тоже должны быть уникальными, => уникальные строки могут быть использованы в качестве
    //ключа в std::map

    //1.2 а вот значения (соответствующие именованым константам)
    //могут быть любыми и даже могут повторяться (упрощаем задачу, считая, что значения НЕ повторяются)
    //=> подходит контейнер std::map<string,<значение> >

    //1.3 Согласно стандарту С++11 переменные перечислимого типа могут быть разного размера и типа,
    //а хотелось бы обеспечить универсальную конвертацию

    //***********
    //2. Так как типы перечислений разные, то enumToString и stringToEnum должны быть шаблонными
    //2.1 Пользователь может ошибиться или злонамеренно передать в функции значения, которым
    //   ничего не соответствует => защита!
    //2.2 В функциях enumToString и stringToEnum (в зависимости от типа перечисления) нужно будет
    //   анализировать соответствующий используемому типу перечисления контейнер

    //***********
    //3. Чтобы действия с map<string, <значение> > не зависили от типа перечисления, логично реализовать "обертку":
    /*
template<typename T> struct EnumMap
{
    static std::map<std::string, T> m_eMap;
    static const auto& getMap() { return m_eMap; }
};
*/
    //***********

    {
    //Например:
/*
        COLORS c1;
        try {
            c1 = stringToEnum<COLORS>("blue");
        }
        catch (...)
        {
        //...
        }


        auto Str = enumToString(c1);
*/
        enum COLORS { red = 1, green = 2, blue = 4 };
        registerEnum<COLORS>({
            { "red", COLORS::red },
            { "green", COLORS::green },
            { "blue" , COLORS::blue }
        });

        COLORS c1{COLORS::red};
        try {
            // Нормальное преобразование
            c1 = stringToEnum<COLORS>("blue");
            cout << "COLORS::blue = " << c1 << endl;
            // Опечатка
            c1 = stringToEnum<COLORS>("blye");
        } catch (const exception& e) {
            cerr << "ERROR: " << e.what() << endl;
        }

        try {
            // Нормальное преобразование
            auto str1 = enumToString(c1);
            cout << "COLORS::blue has name " << str1 << endl;
            // Попытка обмана
            auto str2 = enumToString(static_cast<COLORS>(10));
            cout << str2 << endl;
        } catch (const exception& e) {
            cerr << "ERROR: " << e.what() << endl;
        }

//         enum COL { red = 1, green = 2, blue = 4 };
//         registerEnum<COL>({
//             { "red", COL::red },
//             { "green", COL::green },
//             { "blue" , COL::blue }
//         });
        __asm nop
    }
}

int main() {
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    task7();
    task8();
    task9();
    task10();
    return 0;
}
