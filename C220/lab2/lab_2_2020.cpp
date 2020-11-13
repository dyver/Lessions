#define nop ("nop");

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "MyString.h"

using namespace std;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

template <typename T> void printContainer(const T& c) {
    for (const auto& e : c) {
        cout << e << " ";
    }
    cout << "-> " << size(c);
    cout << endl;
}

template <typename T> class Container {
    public:
        using CT = std::vector<T>;
        Container(
            const std::initializer_list<T>& data, const T& low = T{},
            const T& high = T{}
        )
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
                    if (low == high or (el >= low and el <= high)) {
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
        size_t size() const { return std::size(c); }
        typename CT::iterator begin() { return std::begin(c); }
        typename CT::iterator end() { return std::end(c); }
        typename CT::const_iterator begin() const { return std::cbegin(c); }
        typename CT::const_iterator end() const { return std::cend(c); }
    private:
        CT c;
        const T& low;
        const T& high;
};

template <typename T> class MyQueue {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = const Iterator;

        MyQueue(size_type size, const T& v = T{}) :
            last{size},
            length{size},
            capacity{size}
        {
            data = new value_type[capacity];
            for (size_type i = 0; i < size; ++i) {
                data[i] = v;
            }
        }
        MyQueue(std::initializer_list<T> il) :
            last{0},
            length{il.size()}
        {
            capacity = length * 2;
            data = new value_type[capacity];
            for (const auto& el : il) {
                data[last++] = el;
            }
        }
        MyQueue(const MyQueue& other) :
            first{other.first},
            last{other.last},
            length{other.length},
            capacity{other.capacity}
        {
            data = new value_type[capacity];
            for (size_type i = 0; const auto& el : other) {
                data[i++] = el;
            }
        }
        MyQueue(MyQueue&& other) :
            first{other.first},
            last{other.last},
            length{other.length},
            capacity{other.capacity},
            data{other.data}
        {
            other.first = 0;
            other.last = 1;
            other.length = 0;
            other.capacity = 0;
            other.data = nullptr;
        }
        MyQueue& operator=(std::initializer_list<T> il) {
            first = 0;
            last = 0;
            length = il.size();
            if (capacity < il.size()) {
                capacity = length * 2;
                delete[] data;
                data = new value_type[capacity];
            }
            for (const auto& el : il) {
                data[last++] = el;
            }
            return *this;
        }
        MyQueue& operator=(const MyQueue& other) {
            if (this == &other) return *this;
            first = other.first;
            last = other.last;
            length = other.length;
            capacity = other.capacity;
            delete[] data;
            data = new value_type[capacity];
            for (size_type i = 0; i < capacity; ++i) {
                data[i] = other.data[i];
            }
            return *this;
        }
        MyQueue& operator=(MyQueue&& other) {
            if (this == &other) return *this;
            first = other.first;
            last = other.last;
            length = other.length;
            capacity = other.capacity;
            delete[] data;
            data = other.data;
            other.first = 0;
            other.last = 1;
            other.length = 0;
            other.capacity = 0;
            other.data = nullptr;
            return *this;
        }
        ~MyQueue() {
            if (data) delete[] data;
        }
        bool operator==(const MyQueue& other) const {
            return data == other.data;
        }
        bool operator!=(const MyQueue& other) const {
            return not (*this == other);
        }
        size_type size() const {
            return length;
        }
        bool empty() const {
            return not length;
        }
        value_type pop() {
            if (empty()) throw std::range_error("MyQueue::pop(): queue is empty.");
            value_type result{data[first++]};
            if (first == capacity) first = 0;
            first = first % capacity;
            --length;
            return result;
        }
        void push(const_reference v) {
            ++last;
            ++length;
            if (last > capacity) {
                last = 1;
            }
            if (last == first) {
                size_type new_capacity = length * 2;
                pointer new_data = new value_type[new_capacity];
                size_type current = first;
                size_type new_current = 0;
                while (current + 1 != last) {
                    current = first++ % capacity;
                    new_data[new_current++] = data[current];
                }
                delete[] data;
                data = new_data;
                first = 0;
                last = new_current;
                capacity = new_capacity;
            }
            data[last - 1] = v;
        }
    public:
        class Iterator : public std::iterator<std::forward_iterator_tag, T> {
            friend class MyQueue;
            public:
                Iterator() {}
                Iterator& operator++() {
                    ++current;
                    if (current == holder.last) return *this;
                    current %= holder.capacity;
                    return *this;
                }
                T& operator*() const { return holder.data[current]; }
                bool operator==(const Iterator& other) const {
                    if (holder.length == 0) return true;
                    return holder == other.holder and current == other.current;
                }
                bool operator!=(const Iterator& other) const {
                    return not (*this == other);
                }
            protected:
                Iterator(const MyQueue& holder, size_type current) :
                    holder{holder},
                    current{current}
                {}
                const MyQueue& holder;
                size_type current;
        };
        iterator begin() { return iterator{*this, first}; }
        iterator end() { return iterator{*this, last}; }
        const_iterator begin() const { return iterator{*this, first}; }
        const_iterator end() const { return iterator{*this, last}; }
    private:
        size_type first = 0;
        size_type last = 1;
        size_type length = 0;
        size_type capacity = 10;
        pointer data = nullptr;
};

template <typename T> class MyUniquePTR {
    public:
        using pointer = T*;

        explicit MyUniquePTR(pointer p = nullptr) : ptr{p} {}
        MyUniquePTR(const MyUniquePTR& other) = delete;
        MyUniquePTR(MyUniquePTR&& other) : ptr{other.ptr} {
            other.ptr = nullptr;
        }
        MyUniquePTR& operator=(const MyUniquePTR& other) = delete;
        MyUniquePTR& operator=(MyUniquePTR&& other) {
            if (ptr) delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            return *this;
        }
        ~MyUniquePTR() {
            if (ptr) delete ptr;
        }
        auto operator->() const { return ptr; }
        auto operator*() const { return *ptr; }
        explicit operator bool() const { return ptr; }
        pointer release() { pointer p = ptr; ptr = nullptr; return p; }
    private:
        T* ptr = nullptr;
};

void task1() {
/*
    initializer_list, move, forward, default, delete, move итераторы

    Задание 1. Создайте класс, который должен быть "оберткой" для вектора с
    УНИКАЛЬНЫМИ значениями любого типа в заданном диапазоне.
    Внимание: при инициализации НЕ нужно менять порядок значений,
    заданный пользователем! При наличии повторяющихся значений нужно оставить первое!
    • Для хранения элементов используйте std::vector
    • Реализуйте конструктор, который может принимать любое количество значений
    (значения могут повторяться)
    • Реализуйте метод добавления любого количества значений
    (значения могут повторяться)
    • Реализуйте метод удаления любого количества значений
    (значения могут повторяться)
    • Реализуйте метод сортировки, который будет принимать в качестве параметра
    признак по возрастанию / по убыванию
    • и другие (полезные на Ваш взгляд) методы
*/
    outHeader("1");
    cout << "После конструирования:" << endl;
    Container<int> c({1, 2, 3, 3, 10}, 0, 10);
    printContainer(c);
    cout << "После присваивания:" << endl;
    c = {4, 5, 6, 5, 10};
    printContainer(c);
    cout << "После удаления:" << endl;
    c.remove({6});
    printContainer(c);
    cout << "После повторного удаления:" << endl;
    c.remove({6});
    printContainer(c);
    cout << "После сортировки по возрастанию:" << endl;
    c.sort(true);
    printContainer(c);
    cout << "После сортировки по убыванию:" << endl;
    c.sort(false);
    printContainer(c);
    cout << "Контейнер, содержащий строки:" << endl;
    Container<string> s({ "a"s, "b"s, "b"s, "g"s }, "a"s, "e"s);
    printContainer(s);
    cout << "После сортировки по убыванию:" << endl;
    s.sort(false);
    printContainer(s);
}

void task2() {
/*
    Задание 2. Реализуйте шаблонный класс, который является оберткой для очереди
    с элементами любого типа. Очередь требуется реализовать посредством
    динамического массива, при этом использовать массив как циклический буфер.
    Пояснение: так как очередь – это специфическая структура данных, для которой
    новые данные помещаются в конец, а  «старые» данные изымаются из начала
    очереди => если последний элемент массива задействован, то начало скорее
    всего уже освободилось => «закольцовываем» буфер, продолжая заполнять с
    нулевого элемента.
    Несмотря на указанную специфичность такой структуры данных, могут возникать
    ситуации, когда пользователь вызвал push(), а свободных элементов в очереди
    не осталось => при необходимости массив следует «расширять».
    При реализации нужно обеспечить эффективную работу с динамической памятью=>
    • предусмотреть наличие резервных элементов
    • память без очевидной необходимости не перераспределять
    Внимание!
    1. Очередь реализуем без использования «сырой памяти»! А эффективность
    достигаем за счет использования move-семантики
    2. Очередь выводим на печать с помощью range-base-for
    Тестируем разработанный класс на приведенном ниже фрагменте.
    Следующий фрагмент должен работать не только корректно, но и эффективно:

    MyQueue<MyString>  q1{ MyString(“AAA”), MyString (“qwerty”),<другие_инициализаторы>};
    //использование MyQueue в диапазонном for:
    for (auto& el : q1)  {  std::cout << el << ' '; }
    MyString s(“abc”);
    q1.push(s);
    q1.push(MyString(“123”));
    MyString s1 = q1.pop();
    q1.push(“qqq”);
    MyQueue < MyString >  q2 = q1;
    MyQueue < MyString >  q22 = std::move(q1);

    MyQueue < MyString >  q3{10, MyString (“!”)}; //очередь должна содержать 10 элементов со строкой «!»
    q1 = q3;
    q2 = MyQueue < MyString >  (5, MyString (“?”));
    q1 = { MyString(“bbb”), MyString (“ssss”)};
*/

    outHeader("2");
    MyQueue<MyString> q1{MyString("AAA"), MyString("qwerty")};
    //использование MyQueue в диапазонном for:
    for (const auto& el : q1) { cout << el << ' '; }
    cout << endl;
    MyString s("abc");
    q1.push(s);
    printContainer(q1);
    q1.push(MyString("123"));
    printContainer(q1);
    MyString s1 = q1.pop();
    cout << s1 << endl;
    printContainer(q1);
    q1.push(MyString("qqq"));
    printContainer(q1);
    MyQueue<MyString> q2 = q1;
    printContainer(q2);
    MyQueue<MyString> q22 = std::move(q1);
    printContainer(q1);
    printContainer(q22);
    q22 = std::move(q2);
    printContainer(q2);
    printContainer(q22);
    MyQueue<MyString> q3{10, MyString("!")}; //очередь должна содержать 10 элементов со строкой «!»
    printContainer(q3);
    q1 = q3;
    printContainer(q1);
    q2 = MyQueue<MyString> (5, MyString("?"));
    printContainer(q2);
    q1 = {MyString("bbb"), MyString("ssss")};
    printContainer(q1);
}

void task3() {
/*
    Задание 3. Реализуйте шаблон класса MyUniquePTR, который является оберткой
    для указателя на объект любого типа.
    Задача – класс должен обеспечивать единоличное владение динамически
    создаваемым объектом. Проверить функционирование шаблона на примере MyString:
    {
        MyUniquePTR<MyString> p1(new MyString (“abc”));
        std:: cout<<p1->GetString();
        p1->SetNewString(“qwerty”);
        MyString  s2 = *p1;
        //MyUniquePTR< MyString > p2=p1; //здесь компилятор должен выдавать ошибку =>
        Исправьте!
        If(p1) {std::cout<<”No object!”} //а это должно работать
        MyUniquePTR< MyString > p3(new MyString (“vvv”));
        //p3 = p2; //и здесь компилятор должен выдавать ошибку
        vector< MyUniquePTR< MyString >> v; //как проинициализировать???
        list< MyUniquePTR< MyString >> l;
        //как скопировать из v в l ???
    }
*/

    outHeader("3");
    MyUniquePTR<MyString> p1(new MyString("abc"));
    cout << p1->GetString() << endl;
    p1->SetNewString("qwerty");
    MyString s2 = *p1;
//     MyUniquePTR<MyString> p2 = p1; //здесь компилятор должен выдавать ошибку => Исправьте!
    MyUniquePTR<MyString> p2 = move(p1);
//    If(p1) {std::cout<<”No object!”} //а это должно работать
    if (not p1) { cout << "No object!" << endl; } //а это должно работать
    MyUniquePTR<MyString> p3(new MyString("vvv"));
//     p3 = p2; //и здесь компилятор должен выдавать ошибку
    vector<MyUniquePTR<MyString>> v; //как проинициализировать???
    v.emplace_back(MyUniquePTR<MyString>{new MyString("a")});
    v.emplace_back(MyUniquePTR<MyString>{new MyString("b")});
//     vector<MyUniquePTR<MyString>> v1{
//         move(MyUniquePTR<MyString>{new MyString("a")}),
//         move(MyUniquePTR<MyString>{new MyString("b")})
//     };
    cout << "До перемещения:" << endl;
    list<MyUniquePTR<MyString>> l;
    cout << "    v:" << endl;
    for (const auto& el : v) {
        cout << el->GetString() << endl;
    }
    cout << "    l:" << endl;
    for (const auto& el : l) {
        cout << el->GetString() << endl;
    }
    cout << "После перемещения:" << endl;
    for (auto it = begin(v); it != end(v);) {
        l.emplace_back(it->release());
        it = v.erase(it);
    }
//     for (auto& el : v) {
//         l.emplace_back(el.release());
//     }
    cout << "    v:" << endl;
    for (const auto& el : v) {
        cout << el->GetString() << endl;
    }
    cout << "    l:" << endl;
    for (const auto& el : l) {
        cout << el->GetString() << endl;
    }
}

int main() {
    task1();
    task2();
    task3();
    return 0;
}
