#define nop ("nop");

#include <iostream>
#include <memory>

using namespace std;

enum A { AA, BB, CC };
enum X { AAX, BBX, CCX };

struct C {
    virtual void f(int i) { cout << "C::f:" << i << endl; }
};

struct D : public C {
    virtual void f() { cout << "D::f" << endl; }
};

struct Z {
    int x = 0;
    Z& operator++() { ++x; return *this; }
};

int main() {
//     X x = AA;
    int v[] = {1, 2, 3};
    auto [a, b, c] = v;
    (void)a; (void)b; (void)c;
    pair<int, double> p(1, 1.1);
    auto [x, y] = p;
    cout << x << " " << y << endl;
//     int i = AA;

//     cout << 123 << endl;

    C* d = new D;
    d->f(5);

    Z z;
    cout << z.x << endl;
//     [z] { ++z.x; }();
    cout << z.x << endl;
    [&z] { ++z; }();
    cout << z.x << endl;

    unique_ptr<FILE, void(*)(FILE*)> pc(
        fopen("1.txt", "r"),
        [](FILE* pf) { cout << 123 << endl; fclose(pf); }
    );

    return 0;
}
