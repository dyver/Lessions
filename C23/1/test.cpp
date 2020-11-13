#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>

#include <test.h>

using namespace std;
using namespace std::chrono;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

// milliseconds total_time{0};
microseconds total_time{0};

struct Timer {
    Timer() {
        before = system_clock::now();
    };
    ~Timer() {
//         milliseconds elapsed = duration_cast<milliseconds>(system_clock::now() - before);
//         cout << "elapsed = " << elapsed.count() << " milliseconds" << endl;
        microseconds elapsed = duration_cast<microseconds>(system_clock::now() - before);
        cout << "elapsed = " << elapsed.count() << " microseconds" << endl;
        total_time += elapsed;
    };
    system_clock::time_point before;
};

int main() {

    outHeader("1");

    {

        string filespec[] = { "1.txt", "2.txt", "3.txt", "4.txt" };

        auto processor = [] (const string& name) {
            ifstream ifs{name};
            ofstream ofs{name + "_"};
            transform(
                istreambuf_iterator<char>(ifs),
                istreambuf_iterator<char>(),
                ostreambuf_iterator<char>(ofs),
                [] (char c) { return toupper(c); }
            );
        };

        vector<thread> threads;
        threads.reserve(10);

        for (const auto& f : filespec) {
            Timer a;
            threads.emplace_back(thread{processor, f});
            threads.back().join();
        };
        cout << "Total elapsed = " << total_time.count() << " milliseconds" << endl;

        total_time = 0ms;
        threads.clear();

        for (const auto& f : filespec) {
            Timer a;
            threads.emplace_back(thread{processor, f});
        };
        for (auto& f : threads) {
            f.join();
        };
        cout << "Total elapsed = " << total_time.count() << " milliseconds" << endl;

        total_time = 0ms;
        threads.clear();

        for (auto f = cbegin(filespec); f != cend(filespec); ) {
            for (size_t i = 0; i < thread::hardware_concurrency(); ++i) {
                Timer a;
                threads.emplace_back(thread{processor, *f});
                ++f;
                if (f == cend(filespec)) break;
            }
        };
        for (auto& f : threads) {
            f.join();
        };
        cout << "Total elapsed = " << total_time.count() << " milliseconds" << endl;

        total_time = 0ms;
        threads.clear();

    }

    outHeader("2");

    {

    //     Play("G2", 100);// pl B2 pl D3 pl G3 pl D4 pl G4);
//         thread th1{PlaySequence, "C1D1E1F1G1A1H1C2", 200};
//         thread th2{PlaySequence, "C2H1A1G1F1E1D1C1", 200};
//         th1.detach();
//         th2.detach();
//         this_thread::sleep_for(4s);

    }

    outHeader("3");

    {
        const size_t MaxThreads = thread::hardware_concurrency();

        cout << "We have " << MaxThreads << " concurrent threads." << endl;
        cout << "Input desired number of threads (1-" << MaxThreads << "):";
        size_t threads_number;
        cin >> threads_number;
        if (threads_number < 1) threads_number = 1;
//         if (threads_number > MaxThreads) threads_number = MaxThreads;
        cout << "Ok, we will use " << threads_number << " thread(s)." << endl;

        vector<int> cont1;
        for (int i = 0; i < 10000000; ++i) {
            cont1.push_back(rand() - RAND_MAX / 2);
        }
//         { -1, -2, -5, -4, -11, };
        vector<int> cont2(cont1.size());
        vector<thread> threads;
        size_t tail = 0;
        {
            Timer a;
            if (threads_number > 1) {
                size_t shift = cont1.size() / threads_number;
                for (size_t i = 0; i < threads_number - 1; ++i) {
                    threads.emplace_back(
                        thread{
                            transform_<decltype(cbegin(cont1)), decltype(begin(cont2))>,
                            cbegin(cont1) + shift * i,
                            cbegin(cont1) + shift * (i + 1),
                            begin(cont2) + shift * i
                        }
                    );
                };
                tail = (threads_number - 1) * (cont1.size() / threads_number) + 1;
            }
            thread th_main{
                transform_<decltype(cbegin(cont1)), decltype(begin(cont2))>,
                cbegin(cont1) + tail,
                cend(cont1),
                begin(cont2) + tail
            };
            th_main.join();
            for (auto& f : threads) {
                f.join();
            };
        }
        cout << "Total elapsed = " << total_time.count() << " milliseconds" << endl;
        total_time = 0ms;

//         for (const auto& el : cont2) {
//             cout << el << ", ";
//         }
        cout << endl;
    }

    outHeader("4");
    {
        threadsafe_stack<int> tss;

        auto writer_function = [](threadsafe_stack<int>* tssp) {
            for (int i = 1; i < 10; ++i) {
                tssp->push(i);
                this_thread::sleep_for(milliseconds(i));
            }
        };

        auto reader_function = [](threadsafe_stack<int>* tssp) {
            milliseconds ms(10);
            for (int i = 1; i < 10; ++i) {
                try {
                    int val;
                    tssp->pop(val);
                    cout << val << ", ";
                } catch (const exception& e) {
                    cout << "@";
                    this_thread::sleep_for(1ms);
                }
            }
        };

        thread writer1(writer_function, &tss);
        thread reader1(reader_function, &tss);
        thread writer2(writer_function, &tss);
        thread reader2(reader_function, &tss);

        writer1.join();
        writer2.join();
        reader1.join();
        reader2.join();


    }

//     //SAVE:
//     cout << thread::hardware_concurrency() << endl;
}
