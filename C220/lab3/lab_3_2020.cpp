#define nop ("nop");

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <ctime>
// #include <cstdint>
#include <climits>
// #include <algorithm>
// #include <iterator>
#include <memory>
#include <functional>
#include <cassert>

using namespace std;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

template <typename T> void printContainer(const T& c) {
    for (const auto& e : c) {
        cout << *e << " ";
    }
    cout << "-> " << size(c);
    cout << endl;
}

template <typename T> void printSmartContainer(const T& c) {
    for (const auto& e : c) {
        cout << *e << " ";
    }
    cout << "-> " << size(c);
    cout << endl;
}

////////////////////////////////////////////////////////////////////////////////
// Task 2

constexpr int factorial(int i) {
    if (i == 1) return i;
    return i * factorial(i - 1);
}

////////////////////////////////////////////////////////////////////////////////
// Task 3a

constexpr int strLength(const char* s, const int pos = 0) {
    if (*s == 0) return pos;
    return strLength(s + 1, pos + 1);
}

constexpr uint32_t binStrToInt(const char* s, const int pos) {
    if (pos < 0) return 0;
    return (s[0] - '0') ?
        (1 << pos) + binStrToInt(s + 1, pos - 1) :
        binStrToInt(s + 1, pos - 1);
}

constexpr uint32_t operator""_b(const char* s){
    return binStrToInt(s, strLength(s) - 1);
}

////////////////////////////////////////////////////////////////////////////////
// Task 3b

using ULL = unsigned long long;

string operator""_toBinStr(ULL v) {
    stringstream ss;
    ss << "0b";
    bool skip = true;
    for (int i = (int)sizeof(ULL) * CHAR_BIT - 1; i >= 0 ; --i) {
        auto res = v & (1ULL << i); //v >> i
        if (res) skip = false;
        if (not skip) {
            ss << (res ? 1 : 0);
        } else {
            if (i == 0) ss << (res ? 1 : 0);
        }
    }
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
// Task 4a

template <typename T> class LowHi {
    public:
        LowHi() {}
        constexpr LowHi(const T& low, const T& hi) : low{low}, hi{hi} {}
        void setLow(const T& low) { low = low; }
        constexpr T getLow() const { return low; }
        void setHi(const T& hi) { hi = hi; }
        constexpr T getHi() const { return hi; }
        constexpr T test(const T& val) const {
            if (val > hi) return hi;
            if (val < low) return low;
            return val;
        }
    private:
        T low{};
        T hi{};
};

void task1() {
    // Задание 1. Сырые строковые литералы (Raw String Literals)
    // Выведите строку, например: my name is "Marina"
    // а) без использования Raw String Literals
    // б) посредством Raw String Literals
    // в) добавьте разделители (Delimeter)

    outHeader("1");

    cout << "my name is \"Marina\"" << endl;
    cout << R"(my name is "Marina")" << endl;
    cout << R"=(my name is "Marina")=" << endl;
    cout << R"***("raw string with quotes and brackets")")***" << endl;
}

void task2() {
    ////////////////////////////////////////////////////////////////////////////
    // Задание 2. Реализуйте вычисление факториала с помощью constexpr-функции.
    //
    // Подсказки/напоминания:
    // - constexpr – функция должна состоять из единственной инструкции
    //   return <выражение>; (пока!)
    // - но это выражение может включать вызов другой constexpr – функции,
    //   в частности рекурсивный вызов
    // - если параметр рекурсивной constexpr- функции - это константа,
    //   компилятор вычислит результат на этапе компиляции

    // Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции
    // (если в качестве параметра используется константа, известная
    // компилятору на этапе компиляции).
    // Для проверки достаточно создать встроенный массив с размерностью,
    // вычисляемой посредством constexpr-функции:

    //Например:

    outHeader("2");

    cout << factorial(3) << endl;
    int ar[factorial(3)];
    (void)ar;

    //или
    constexpr int n = factorial(5);
    int ar1[n];
    (void)ar1;

    //попробуйте:
    int m = 7;
//     constexpr int n1 = factorial(m);
//     int ar1[n1];

    //а так?
    int n2 = factorial(m);
    (void)n2;
}

void task3a() {
    ////////////////////////////////////////////////////////////////////////////
    // Задание 3a. Перевод с помощью пользовательского литерала из двоичного
    // представления строкового в значение, например: строку "100000000" ->
    // в значение 256.
    // Проверить результат посредством префикса 0b
    // Чтобы вызов пользовательского литерала выглядел просто и читаемо,
    // например: 100000000_b логично использовать пользовательский литерал
    // с единственным параметром - const char*

    // Так как речь идет о литералах, логично вычислять значения на этапе
    // компиляции => реализуйте пользовательский литерал посредством
    // constexpr - функций
    // Подсказка/напоминание:
    // - constexpr – функция должна состоять из единственной инструкции
    //   return <выражение>;
    // - но это выражение может включать вызов другой constexpr – функции,
    // - которая может быть рекурсивной (если параметр такой функции -
    //   это константа, компилятор вычислит результат вызова рекурсивной
    //   функции на этапе компиляции)

    outHeader("3a");

    auto result = 1001_b;
    assert(result == 0b1001);
    cout << result << endl;
    result = 1111_b;
    assert(result == 0b1111);
    cout << result << endl;
}

void task3b() {
    // Задание 3b. Перевод в строковое двоичное представление,
    // например: 256 -> "0b100000000"
    // Так как строка может быть любой длины, логичнее и проще возвращать
    // объект std::string => возвращаемое значение не может быть constexpr!
    // Подсказка: манипулятора std::bin пока нет => преобразование
    // в двоичную строку придется делать вручную
    // Подсказка: количество разрядов в байте определяет константа
    // CHAR_BIT - <cstdint>

    outHeader("3b");

    std::string sBin = 255_toBinStr;
    assert(sBin == "0b11111111");
    cout << sBin << endl;
    sBin = 0_toBinStr;
    assert(sBin == "0b0");
    cout << sBin << endl;
}

void task4a() {
    ////////////////////////////////////////////////////////////////////////////
    // Задание 4а. constexpr - объекты
    // Создать класс (шаблон класса?) для хранения и манипулирования
    // диапазоном значений.
    // В классе должны быть:
    //   переменные для хранения минимального и максимального значений,
    //   методы для получения каждого из значений
    //   метод для проверки - попадает ли указанное значение в диапазон
    //   метод, который получает любое значение данного типа и формирует
    //   результирующее значение:
    //      если принадлежит диапазону, то его и возвращаем
    //      если меньше минимального значения, возвращаем минимальное
    //      если больше максимального значения, возвращаем максимальное

    // Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции.
    //   Для проверки достаточно создать встроенный массив с размерностью,
    //   вычисляемой посредством constexpr-метода:

    outHeader("4a");

    constexpr LowHi<int> lh{-100, 100};
    constexpr int low = lh.getLow();
    constexpr int hi = lh.getHi();
    constexpr int test = lh.test(666);

//     int a1[low];
//     (void)a1;
    int a2[hi];
    (void)a2;
    int a3[test];
    (void)a3;

    assert(low == -100);
    assert(hi == 100);
    assert(test == 100);

    cout << "low: " << low << endl;
    cout << "hi: " << hi << endl;
    cout << "test(100): " << test << endl;
}

////////////////////////////////////////////////////////////////////////////
// Задание 5. unique_ptr

void task5a() {
    // 5.а - обеспечьте корректное выполнение фрагмента
    outHeader("5a");
    {
        std::vector<std::string*> v = {
            new std::string("aa"), new std::string("bb"), new std::string("cc")
        };
        //Распечатайте все строки
        for (const auto& i : v) {
            cout << *i << " ";
        }
        cout << endl;
//         for (const auto& i : v) {
//             delete i;
//         }
        for (auto& i : v) {
            delete i;
            i = nullptr;
        }
    } //???
}

void task5b() {
    // 5.b - модифицируйте задание 5.а:
    // обеспечьте посредством std::unique_ptr:
    // эффективное заполнение (вспомните про разные способы формирования
    // std::unique_ptr), безопасное хранение указателей на динамически
    // создаваемые объекты std::string, манипулирование и освобождение ресурсов
    outHeader("5b");
    {
        vector<unique_ptr<string>> v;
        v.emplace_back(new string("aa"));
        v.push_back(make_unique<string>("bb"));
        v.push_back(unique_ptr<string>(new string("cc")));

        //Распечатайте все строки
        printSmartContainer(v);

        string s{"9999"};
        v[0] = make_unique<string>(move(s));
        printSmartContainer(v);

        //??? Уничтожение динамически созданных объектов?
    } //???
}

void task5c() {
    // 5.c - дополните задание 5.b добавьте возможность изменять хранящиеся строки
    // следующим образом (например, добавить указанный суффикс: "AAA" -> "AAA_1")
    outHeader("5c");
    {
        vector<unique_ptr<string>> v;
        v.emplace_back(new string("aa"));
        v.push_back(make_unique<string>("bb"));
        v.push_back(unique_ptr<string>(new string("cc")));

        for_each(begin(v), end(v), [](auto& el) { *el = move(*el + "_suffix"); });

        printSmartContainer(v);
    }
}

void task5d() {
    // 5.d - динамический массив объектов
    // Создайте unique_ptr, который является оберткой для динамического массива
    // с элементами std::string
    // С помощью unique_ptr::operator[] заполните обернутый массив значениями
    // Когда происходит освобождения памяти?
    outHeader("5d");
    {
        int n = 10;
        unique_ptr<
            string[],
            function<void(string[])>
        > p(
            new string[n],
            [](string data[]) { if (data) delete[] data; }
        );

        for (int i = 0; i < n; ++i) {
            p[i] = string(to_string(i));
        }

        for (int i = 0; i < n; ++i) {
            cout << p[i] << " ";
        }
        cout << endl;
    }
}

void task5e() {
    // 5.e - массивы динамических объектов и пользовательская delete-функция
    // (функтор)
    // Задан стековый массив указателей на динамически созданные объекты
    // Создайте unique_ptr для такого массива
    // Реализуйте пользовательскую delete-функцию (функтор) для корректного
    // освобождения памяти
    outHeader("5e");
    {
        string* ar[] = {
            new std::string("AA"), new std::string("BB"), new std::string("CC")
        };

        int n = sizeof(ar);

        unique_ptr<
            string*[],
            function<void(string*[])>
        > p(
            ar,
            [n](string* data[]) {
            for (int i = 0; i < n; ++i) {
                if (data[i]) delete data[i];
            }
        });

        for (int i = 0; i < n; ++i) {
            cout << *p[i] << " ";
        }
        cout << endl;
    }
}

void task5f() {
    // 5.f Создайте и заполните вектор, содержащий unique_ptr для указателей
    // на std::string
    // Посредством алгоритмя copy() скопируйте элементы вектора в пустой список
    // с элементами того же типа
    // Подсказка: перемещающие итераторы и шаблон std::make_move_iterator
    outHeader("5f");
    {
        vector<unique_ptr<string>> v;
        v.push_back(make_unique<string>("aaa"));
        v.push_back(make_unique<string>("bbb"));
        v.push_back(make_unique<string>("ccc"));

        printSmartContainer(v);

        list<unique_ptr<string>> l;

        copy(make_move_iterator(begin(v)),
            make_move_iterator(end(v)),
            inserter(l, begin(l))
        );
        printSmartContainer(l);
    }
}

void task6() {
    ////////////////////////////////////////////////////////////////////////////
    // Задание 6.shared_ptr + пользовательская delete-функция

    // Реализовать возможность записи в файл данных (строчек) из разных источников
    // (для упрощения пусть источниками являются два массива)
    // Так как все "писатели" будут по очереди записывать свои данные в один
    // и тот же файл, логично предоставить им возможность пользоваться одним и
    // тем же указателем FILE* =>
    // безопасной оберткой для такого указателя является shared_ptr
    // а. Первый владелец должен открыть/создать файл для записи
    // б. Все остальные писатели должны присоединиться к использованию
    // в. Последний владелец указателя должен закрыть файл

    // Подсказка: имитировать порядок записи можно с помощью функции rand()

    outHeader("6");
    {
        // "писатели":
        // Создать writer1, writer2
        auto writer1 = shared_ptr<FILE>(
            fopen("out.txt", "w"),
            [](FILE* f) { fclose(f); }
        );
        auto writer2 = writer1;

        // например, источники данных:
        char ar1[] = "Writer1\n";
        char ar2[] = "Writer2\n";

        // заданное число итераций случайным образом позволяем одному из "писателей"
        // записать в файл свою строчку
        // Подсказка: строчки удобно записывать в файл посредством функции fputs()

        int n = 10;

        srand(time(NULL));
        for (int i = 0; i < n; ++i) {
            if (rand() % 2) {
                fputs(ar1, writer1.get());
            }
            else {
                fputs(ar2, writer2.get());
            }
        }
    }//закрытие файла???

    ifstream f("out.txt");
    if (f.is_open()) {
        cout << f.rdbuf() << endl;
        f.close();
    }
}

int main() {
    task1();
    task2();
    task3a();
    task3b();
    task4a();
    task5a();
    task5b();
    task5c();
    task5d();
//     task5e();
    task5f();
    task6();
    return 0;
}
