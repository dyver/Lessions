#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include <u8_lab_2.h>

using namespace std;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

void task1() {
    outHeader("1");

    MyArray<string> marr{ "1"s, "2"s, "3"s };
    auto s1{"A"s};
    marr.push(s1);
    marr.push("B"s);
    auto e1 =  marr.pop();
    cout << e1 << endl;
    auto marr2{marr};
    cout << "marr.size=" << marr.length() << " marr2.size=" << marr2.length() << endl;
    MyArray<string> marr3(10);
    cout << "marr3.size=" << marr3.length() << endl;
    auto marr4{move(marr)};
    cout << "marr.size=" << marr.length() << " marr4.size=" << marr4.length() << endl;
    marr = marr2 = marr3;
    marr3 = move(marr = move(marr2));
    try {
        while (true) {
            cout << marr4.pop() << endl;
        }
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
    }

    MyArray<vector<int>> bigdata;
    bigdata.push(vector<int>(10'000'000));
}

template <typename T> void outContainer(const T& c) {
    for (const auto& el : c) {
        cout << el << " ";
    }
    cout << endl;
}

void task2() {

    outHeader("2");

    cout << "constructed by size: ";
    MyList<int> ml0(10, 5);
    outContainer(ml0);

    cout << "constructed from initializer list: ";
    MyList<int> ml1{ 1, 2, 3 };
    outContainer(ml1);

    cout << "constructed from copy: ";
    auto ml2{ml1};
    outContainer(ml2);

    cout << "constructed by move: ";
    auto ml3{move(ml2)};
    outContainer(ml3);

    cout << "copied from initializer list: ";
    ml2 = { 10, 20, 30 };
    outContainer(ml2);

    cout << "copied: ";
    ml2 = ml3;
    outContainer(ml2);

    cout << "moved: ";
    ml2 = move(ml3);
    outContainer(ml2);

    cout << "modified: ";
    for (auto& el : ml1) {
        el *= 100;
    }
    outContainer(ml1);

    cout << "assigned initializer list: ";
    ml2.assign({ 11, 22, 33 });
    outContainer(ml2);

    cout << "assigned number of equal values: ";
    ml2.assign(size_t(10), 9);
    outContainer(ml2);

    cout << "assigned from iterators: ";
    int m[] = { 81, 82, 83, 84 };
    ml2.assign(cbegin(m), cend(m));
    outContainer(ml2);

    cout << "element added to front: ";
    ml1.push_front(111);
    outContainer(ml1);

    cout << "element added to back: ";
    ml1.push_back(999);
    outContainer(ml1);

    cout << "reversed: ";
    for (auto it = crbegin(ml1); it != crend(ml1); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "front=" << ml1.front() << ", back=" << ml1.back() << endl;

    try {
        ml2.clear();
        cout << ml2.front() << endl;
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }

    cout << "standard algorithms: ";
    for_each(begin(ml1), end(ml1), [](int i) { cout << i << " "; });
    cout << endl;
    auto result = find(begin(ml1), end(ml1), 999);
    if (result != ml1.end()) {
        cout << "value found." << endl;
    } else {
        cout << "value not found." << endl;
    }

}

void task21() {
    try {
        Any a{1};
        Any b = 5;
        cout << "a + b = " << a + b << endl;
        Any c = b;
        cout << "c = " << c << endl;
        Any d{move(c)};
        cout << "d = " << d << endl;
        a = move(d);
        cout << "a = " << a << endl;
        cout << "a + 1 = " << a + 1 << endl;
        cout << "c = " << c << endl;
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

int main() {
    task1();
    task2();
    task21();
    return 0;
}
