#define nop ("nop");

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <random>
#include <cstdint>
#include <cstdio>

#include <u8_lab_3.h>

using namespace std;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

int main() {

    //Задание 1. Сырые строковые литералы (Raw String Literals)
    //Выведите строку, например: my name is "Marina"
    //а) без использования Raw String Literals
    //б) посредством Raw String Literals
    //в) добавьте разделители (Delimeter)
    {
        outHeader("1");

        cout << "my name is \"Marina\"" << endl;
        cout << R"(my name is "Marina")" << endl;
        cout << R"==(my name is "Marina")==" << endl;

        __asm nop
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 2. Реализуйте вычисление факториала с помощью constexpr-функции.
    //
    //Подсказки/напоминания:
    //        - constexpr – функция должна состоять из единственной инструкции return <выражение>; (пока!)
    //        - но это выражение может включать вызов другой constexpr – функции, в частности рекурсивный
    //            вызов
    //        - если параметр рекурсивной constexpr- функции - это константа, компилятор вычислит результат
    //            на этапе компиляции

    //Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции (если в качестве
    //                параметра используется константа, известная компилятору на этапе компиляции).
    //                Для проверки достаточно создать встроенный массив с размерностью, вычисляемой
    //                посредством constexpr-функции:

    {//Например:
        outHeader("2");

        int ar[factorial(3)];
        cout << sizeof(ar) / sizeof(ar[0]) << endl;
        //или
        constexpr int n = factorial(5);
        int ar1[n];
        cout << sizeof(ar1) / sizeof(ar1[0]) << endl;

        //попробуйте:
        int m = 7;
        // запрещено
//         constexpr int n1 = factorial(m);
//         int ar2[n1];

        //а так?
        int n2 = factorial(m);
        cout << n2 << endl;

        __asm nop
    }

    //////////////////////////////////////////////////////////////////////////////////////////////

    //Задание 3a. Перевод с помощью пользовательского литерала из двоичного представления строкового
    //в значение, например: строку "100000000" -> в значение 256
    //Проверить результат посредством префикса 0b
    //Чтобы вызов пользовательского литерала выглядел просто и читаемо, например: 100000000_b
    //логично использовать пользовательский литерал с единственным параметром - const char*

    //Так как речь идет о литералах, логично вычислять значения на этапе компиляции
    // => реализуйте пользовательский литерал посредством constexpr - функций
    //Подсказка/напоминание:
    //        - constexpr – функция должна состоять из единственной инструкции return <выражение>;
    //        - но это выражение может включать вызов другой constexpr – функции,
    //        - которая может быть рекурсивной (если параметр такой функции - это константа,
    //                    компилятор вычислит результат вызова рекурсивной функции на этапе компиляции)

    {
        outHeader("3a");

        cout << 0b100000011 << endl;
        cout << 100000011_b << endl;

        __asm nop
    }

    //Задание 3b. Перевод в строковое двоичное представление, например: 256 -> "0b100000000"
    //Так как строка может быть любой длины, логичнее и проще возвращать объект std::string
    //=> возвращаемое значение не может быть constexpr!
    //Подсказка: манипулятора std::bin пока нет => преобразование в двоичную строку
    //придется делать вручную
    //Подсказка: количество разрядов в байте определяет константа CHAR_BIT - <cstdint>

    {
        outHeader("3b");

        cout << 0b100000001 << endl;
        std::string sBin = 257_toBinStr;
        cout << sBin << endl;

        __asm nop
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 4а. constexpr - объекты
    //Создать класс (шаблон класса?) для хранения и манипулирования диапазоном значений.
    //В классе должны быть:
    //    переменные для хранения минимального и максимального значений,
    //    методы для получения каждого из значений
    //    метод для проверки - попадает ли указанное значение в диапазон
    //    метод, который получает любое значение данного типа и формирует результирующее значение:
    //                            если принадлежит диапазону, то его и возвращаем
    //                            если меньше минимального значения, возвращаем минимальное
    //                            если больше максимального значения, возвращаем максимальное

    //Проверьте тот факт, что компилятор вычисляет значение на этапе компиляции.
    //                Для проверки достаточно создать встроенный массив с размерностью, вычисляемой
    //                посредством constexpr-метода:
    {
        outHeader("4a");

        constexpr Diapason<int> di(1, 10);
        ConstexprChecker<di.getLow()> cc1;
        ConstexprChecker<di.getHigh()> cc2;
        ConstexprChecker<di.toBounds(0)> cc3;
        ConstexprChecker<di.isIn(5)> cc4;
        cout << "low=" << di.getLow() << ", high=" << di.getHigh() << endl;
        cout << "5 is in ? " << (di.isIn(5) ? "yes" : "no") << endl;

        __asm nop
    }


        //Задание 4b. constexpr класс с битовыми полями
        //Часто в задачах требуется получать разные наборы констант в зависимости от
        //разных условий. Причем диапазон значений констант может быть существенно меньше
        //диапазона значений, предоставляемых типом char или unsigned char

        //Посредством constexpr класса и битовых полей реализуйте получение и
        //использование констант на этапе компиляции

    {
        outHeader("4b");

        constexpr Bits<3> bits{ 1, 2, 3 };
        cout << bits.first() << endl;
        ConstexprChecker<bits.second()> cc2;

        __asm nop
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //Задание 5. unique_ptr
    {

        //5.а - обеспечьте корректное выполнение фрагмента
        {
            outHeader("5a");

            std::vector<std::string*> v = { new std::string("aa"), new std::string("bb"), new std::string("cc") };
            //Распечатайте все строки
            printContainer(v);

            for (const auto& el : v) {
                delete el;
            }

            __asm nop
            //???
        } //???

        //5.b - модифицируйте задание 1.а:
         //обеспечьте посредством std::unique_ptr:
         //эффективное заполнение (вспомните про разные способы формирования std::unique_ptr),
         //безопасное хранение указателей на динамически создаваемые объекты std::string,
         //манипулирование,
         //и освобождение ресурсов
         //

        using ContainerElementType = std::unique_ptr<std::string>;
        using ContainerType = std::vector<ContainerElementType>;

        {
            outHeader("5b");

            ContainerType v;
            v.push_back(make_unique<string>("aa"));
            v.push_back(make_unique<string>("bb"));
            v.push_back(make_unique<string>("cc"));

         //Распечатайте все строки
            printContainer(v);

            __asm nop
         //??? Уничтожение динамически созданных объектов?
        } //???

        {//5.c - дополните задание 5.b добавьте возможность изменять хранящиеся строки
         //следующим образом (например, добавить указанный суффикс: "AAA" -> "AAA_1")
            outHeader("5c");

            ContainerType v;
            v.push_back(make_unique<string>("aa"));
            v.push_back(make_unique<string>("bb"));
            v.push_back(make_unique<string>("cc"));

            appendSuffix(v, "_1");

            printContainer(v);

            __asm nop
        }

        {//5.d - динамический массив объектов
            outHeader("5d");

         //Создайте unique_ptr, который является оберткой для динамического массива
         //с элементами std::string
         //С помощью unique_ptr::operator[] заполните обернутый массив значениями
         //Когда происходит освобождения памяти?
            const int length = 10;
            auto deleter = [](string* p) { delete[] p; };
            unique_ptr<string[], decltype(deleter)> vc(new string[length], deleter);
            for (int i = 0; i < length; ++i) {
                vc[i] = to_string(i);
            }
            for (int i = 0; i < length; ++i) {
                cout << vc[i] << " ";
            }
            cout << endl;

            __asm nop
        }

        {//5.e - массивы динамических объектов и пользовательская delete-функция (функтор)
         //Задан стековый массив указателей на динамически созданные объекты
         //Создайте unique_ptr для такого массива
         //Реализуйте пользовательскую delete-функцию (функтор) для корректного
         //освобождения памяти
            outHeader("5e");

            struct Wrapper {
                std::string** p;
                size_t size;
            };

            std::string* arStrPtr[] = { new std::string("aa"), new std::string("bb"), new std::string("cc") };

            Wrapper wrapper{arStrPtr, sizeof(arStrPtr) / sizeof(arStrPtr[0])};
            auto deleter = [](decltype(wrapper)* p) {
                for (size_t i = 0; i < p->size; ++i) {
                    delete *(p->p + i);
                }
            };
            unique_ptr<decltype(wrapper), decltype(deleter)> vc(&wrapper, deleter);

            __asm nop
        }

        {//5.f Создайте и заполните вектор, содержащий unique_ptr для указателей на std::string
            //Посредством алгоритмя copy() скопируйте элементы вектора в пустой список с элементами
            //того же типа
            //Подсказка: перемещающие итераторы и шаблон std::make_move_iterator
            outHeader("5f");

            vector<unique_ptr<string>> v;
            using IT = decltype(v)::iterator;
            v.push_back(make_unique<string>("a"));
            v.push_back(make_unique<string>("b"));
            v.push_back(make_unique<string>("c"));
            list<unique_ptr<string>> l;
            copy(make_move_iterator<IT>(begin(v)), make_move_iterator<IT>(end(v)), back_inserter(l));
            printContainer(l);

            __asm nop

        }
        __asm nop

    }

    ////////////////////////////////////////////////////////////////////////////////
    //Задание 6.shared_ptr + пользовательская delete-функция

    //Реализовать возможность записи в файл данных (строчек) из разных источников
    //(для упрощения пусть источниками являются два массива)
    //Так как все "писатели" будут по очереди записывать свои данные в один и тот же файл,
    //логично предоставить им возможность пользоваться одним и тем же указателем FILE* =>
    //безопасной оберткой для такого указателя является shared_ptr
    //а. Первый владелец должен открыть/создать файл для записи
    //б. Все остальные писатели должны присоединиться к использованию
    //в. Последний владелец указателя должен закрыть файл

    //Подсказка: имитировать порядок записи можно с помощью функции rand()

    {
        outHeader("6");

        auto deleter = [](FILE* pf) { fclose(pf); };
        using ST = shared_ptr<FILE>;

    //"писатели":
    //Создать writer1, writer2
        auto writer1 = [](ST pf, char* data) {
            cout << "use_count=" << pf.use_count() << endl;
            fputs(data, pf.get());
            fputs("\n", pf.get());
        };
        auto writer2 = [](ST pf, char* data) {
            cout << "use_count=" << pf.use_count() << endl;
            fputs(data, pf.get());
            fputs("\n", pf.get());
        };

    //например, источники данных:
        char ar1[] = "Writer1";
        char ar2[] = "Writer2";

        ST pf(fopen("temp.txt", "wb"), deleter);
        uniform_int_distribution<int> one_or_two{1, 2};
        default_random_engine re;
        const int times = 10;
        if (one_or_two(re) == 1) {
            for (int i = 0; i < times; ++i) {
                writer1(pf, ar1);
            }
        } else {
            for (int i = 0; i < times; ++i) {
                writer2(pf, ar2);
            }
        }

    //заданное число итераций случайным образом позволяем одному из "писателей" записать в файл
    //свою строчку
    //Подсказка: строчки удобно записывать в файл посредством функции fputs()


    __asm nop
    }//закрытие файла???

}
