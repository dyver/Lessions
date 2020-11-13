#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <future>
#include <thread>

#include <test.h>

using namespace std;
using namespace std::chrono;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

milliseconds total_time{0};

struct Timer {
    Timer() {
        before = system_clock::now();
    };
    ~Timer() {
        milliseconds elapsed = duration_cast<milliseconds>(system_clock::now() - before);
        cout << "elapsed = " << elapsed.count() << " milliseconds" << endl;
        total_time += elapsed;
    };
    system_clock::time_point before;
};


int main() {

    {
        outHeader("2");

        LockFreeStack<int> st;

        vector<thread> threads;

        for (int i = 0; i < 10; ++i) {
            threads.push_back(thread([&] { st.push(i); }));
            threads.push_back(thread([&] { st.push(i); }));
        }

        for (auto& th : threads) {
            th.join();
        }

    }
}
