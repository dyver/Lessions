#define nop ("nop");

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <iostream>
#include <utility>
#include <iterator>
// #include <regex>
#include <cmath>

#include <u8_lab_4.h>

using namespace std;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

int main() {
/*
    //////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 1. Регулярные выражения


    // 1a Обеспечить прием от пользователя целых значений в десятичной форме.
    // Необходимо учесть специфику пользователя, который должен ввести целое,
    //                а может ввести все, что угодно,
    //                число может предворяться знаком -/+,
    //                количество цифр может быть любым (в частности может превышать sizeof(int))
    //
    //Подсказки:
    //            a) std::string может принять строчку с любым содержимым
    //            б) нужно проанализировать, соответствует ли введенное значение целому в допустимом диапазоне
    //            в) и, если соответствует, перевести из строкового представления в значение типа int
    //                    (для преобразования удобно использовать С++11 - std::stoi())

    {
        outHeader("1a");

//         try {
//             cout << "Введите целое число:";
//             string input;
//             cin >> input;
//             cout << parseInteger(input) << endl;
//         } catch (const exception& e) {
//             cout << "ERROR:" << e.what() << endl;
//         }
        const char* samples[] = { "+123", "-456", "0123", "0.3", "123123123123123" };
        for (const auto& el : samples) {
            try {
                cout << parseNumber<int>(el, NumberType::Integer) << endl;
            } catch (const exception& e) {
                cout << "ERROR:" << e.what() << endl;
            }
        }
    }
    // 1b ООбеспечить прием от пользователя целых значений в шестнадцатеричной форме.
    {
        outHeader("1b");

        const char* samples[] = { "0x123", "0xffff", "-456", "0.3", "123123123123123" };
        for (const auto& el : samples) {
            try {
                cout << parseNumber<int>(el, NumberType::Hexadecimal) << endl;
            } catch (const exception& e) {
                cout << "ERROR:" << e.what() << endl;
            }
        }

    }


    // 1с Обеспечить прием от пользователя плавающих значений в форматах
    //      -2.33 или +1.23e+06 или 0.245E10
    {
        outHeader("1c");

        const char* samples[] = { "-2.33", "+1.23e+06", "0.123", ".3", "123123123123123" };
        for (const auto& el : samples) {
            try {
                cout << parseNumber<double>(el, NumberType::Float) << endl;
            } catch (const exception& e) {
                cout << "ERROR:" << e.what() << endl;
            }
        }
    }

    // 1d Требуется узнать - есть ли хотя бы одна десятичная цифра в строке
    {
        outHeader("1d");

        cout << isDecimalDigitIn("asdvknllvanlrsnvr3fdfgfsGRGE") << endl;
        cout << isDecimalDigitIn("asdvknllvanlrsnvrfdfgfsGRGE") << endl;

        __asm nop
    }


    //1e Требуется найти все десятичные цифры в строке
    {
        outHeader("1e");

        for (const auto& el : getAllDecimalDigits("cdssdv5fds49sfdfsd6dsffd")) {
            cout << el << " ";
        }
        cout << endl;

    }

    ////////////////////////////////////////////////////////////////////////////////
    //Задание 2 variadic template - функция
    //Реализуйте функцию, которая умеет печатать любое количество параметров любого типа,
    //Например: MyPrint(4,8.8, A(1),std::string("dfg") );
    {
        outHeader("2");

        MyPrint(4, 8.8, A(1), std::string("dfg"));
    }

    ////////////////////////////////////////////////////////////////////////////////
    //Задание 3 function
    //3a Реализуйте "калькулятор", который будет выполнять простейшие действия: +,-,*,/,^
    //     над операндами типа double:
    // '+' - посредством шаблона глобальной функции
    // '-' - посредством шаблона std::minus
    // '*' - посредством функционального объекта
    // '/' - посредством лямбда-функции
    // '^' - посредством функции pow() (перегруженный вариант для double)
    // '%' - дробную часть от результата плавающего деления посредством std::bind()
    // '?' - наибольшее из двух значений посредством std::bind() и метода Вашего класса
    //
    //Для хранения значка операции и соответствующего ему действия логично использовать
    //std::map<char, ???>
    {
        outHeader("3");

        Calculator c;

        try {
            cout << c.calculate(L"990 999 3 3.9 2 3 ± + ^ - ? %") << endl;
        } catch (const exception& e) {
            cerr << "ERROR:" << e.what() << endl;
        }

        __asm nop
    }


    //3б Рекурсивная лямбда функция. Посредством std::function и лямбда функции реализуйте
    //вычисление суммы натурального ряда (x+(x-1)+ (x-2)+...+1).
    //Подсказка: так как существование любой переменной начинается с момента определения,
    //например: int x=x; - бессмысленно, но корректно
    // => переменную типа function можно использовать в списке захвата  - ... f = [&f]...
    {
        outHeader("3b");

        std::function<int(int)> naturals = [&naturals] (int x) {
            return x ? x + naturals(x - 1) : 0;
        };
        cout << naturals(10000) << endl;
    }

    //////////////////////////////////////////////////////////////////////////////////
    //Задание 4 std::tuple
    //Создайте класс book (автор, заглавие, год издания...).
    //Создайте библиотеку таким образом, чтобы книги хранились в упорядоченном виде.
    //Чтобы упорядочить совокупность книжек, нужно сформировать условие сравнения, то есть
    //перегрузить, например operator<. При реализации оператора хочется сформировать условие
    //с учетом всех данных!!!

    //Подсказка: Для упорядочения удобно использовать шаблон std::tuple,
    //так как для std::tuple перегружены операторы ==,!=,<,<=,>,>=
    //,которые сравнивают два кортежа лексиграфически (в порядке следования членов).


    //Для проверки распечатайте библиотеку

    {
        outHeader("4");

        class Book {
            public:
                Book(std::string author, std::string title, int year) :
                    author{author}, title{title}, year{year}
                {}
                bool operator<(const Book& other) const {
                    return std::make_tuple(author, title, year)
                    < std::make_tuple(other.author, other.title, other.year);
                }
                std::string author;
                std::string title;
                int year;
        };

        std::set<Book> library{
            { "Кэролл Льюис", "Охота на снарка", 1876 },
            { "Кэролл Льюис", "Алиса в стране чудес", 1864 },
            { "Алигьери Данте", "Божественная комедия", 1321 }
        };

        for (const auto& el : library) {
            cout << "Автор: «" << el.author << "», ";
            cout << "Заглавие: «" << el.title << "», ";
            cout << "Год издания: " << el.year << ".";
            cout << endl;
        }
    }
*/
/////////////////////////////////////////////////////////////////
    //Задание 5. shared_ptr и weak_ptr
    //Создаем генеалогическое дерево посредством класса human. В классе хранятся:
    //имя - string
    //возможно признак: жив или уже нет...
    //родители - shared_ptr (родители не всегда известны...)
    //дети - контейнер из weak_ptr (чтобы избежать циклических зависимостей)

    //Методы класса human:
    //конструктор - для инициализации имени и признака
    //конструктор копирования, оператор присваивания, move ???
    //статический метод child() -
    //                должен создать создать и вернуть обертку для родившегося человека
    //                + сформировать все связи ребенка с родителями и наоборот

    //Ввести возможность распечатать генеалогическое дерево для указанного индивидума

    {
        class Human {
            public:
                Human(const std::string& name, bool alive) :
                    name{name}, alive{alive}
                {}
                static std::shared_ptr<Human> registerChild(
                    const std::string& name,
                    bool alive,
                    std::shared_ptr<Human> father,
                    std::shared_ptr<Human> mother
                ) {
                    auto child = shared_ptr<Human>{new Human{name, alive}};
                    child->father = father;
                    child->mother = mother;
                    if (father) father->children.push_back(child);
                    if (mother) mother->children.push_back(child);
                    return child;
                }
                void printAll() {
                    printMyself();
                    printAncestors();
                    printDescendants();
                }
                void printMyself(int level = -1) {
                    ++level;
                    outIdent(level);
                    cout << "My name is " << name << endl;
                }
                void printAncestors(int level = -1) {
                    ++level;
                    if (father) {
                        outIdent(level);
                        cout << "My father :" << endl;
                        father->printMyself(level);
                        father->printAncestors(level);
                    }
                    if (mother) {
                        outIdent(level);
                        cout << "My mother :" << endl;
                        mother->printMyself(level);
                        mother->printAncestors(level);
                    }
                }
                void printDescendants(int level = -1) {
                    ++level;
                    if (children.size()) {
                        outIdent(level);
                        cout << "My children :" << endl;
                        for (const auto& child : children) {
                            child.lock()->printMyself(level);
                            child.lock()->printDescendants(level);
                        }
                    }
                }
                void outIdent(int level) {
                    for (int i = 0; i < level; ++i) {
                        cout << "    ";
                    }
                }
            private:
                std::string name;
                bool alive;
                std::shared_ptr<Human> father;
                std::shared_ptr<Human> mother;
                std::vector<std::weak_ptr<Human>> children;
        };

        //История должна с кого-то начинаться => "Жили-были дед да баба, например, Адам и Ева"
        //(то есть на самом деле два деда и две бабы):


        //std::shared_ptr<human> grandM1(new human("Eva"));
        //...

        shared_ptr<Human> Adam{new Human{"Adam", false}};
        shared_ptr<Human> Eva{new Human{"Eva", false}};

        //у них появились дети - child():

        shared_ptr<Human> Kain = Human::registerChild("Kain", false, Adam, Eva);
        shared_ptr<Human> Avel = Human::registerChild("Avel", false, Adam, Eva);
        shared_ptr<Human> Sif = Human::registerChild("Sif", false, Adam, Eva);

        //а у детей в свою очередь свои дети:

        shared_ptr<Human> Abram = Human::registerChild("Abram", false, Sif, nullptr);
        shared_ptr<Human> Sara = Human::registerChild("Sara", false, Sif, nullptr);

        Sif->printAll();


        //...
        __asm nop
    }


}
