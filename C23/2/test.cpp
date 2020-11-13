#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <future>

#include <test.h>

using namespace std;
using namespace std::chrono;

void outHeader(const char* taskName) {
    cout << endl << "+++++ Задание №" << taskName << "." << endl << endl;
}

const int ThreadPool::MaxThreads = std::thread::hardware_concurrency();

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


// Task #1

thread_local int A::numberOfObjects(0);

struct threadInfo {
    string name;
    int number;
    int numberOfObjects;
};

threadInfo threadFunction(int threadNumber) {

    std::cout << "A::numberOfObjects=" << A::numberOfObjects << endl;

    vector<A> aObjects;

    int numberOfObjects = rand() % 5 + 1;
    for (int i = 0; i < numberOfObjects; ++i) {
        aObjects.emplace_back(A());
    }
    auto info = threadInfo(
        {
            "Thread #" + to_string(threadNumber),
            threadNumber,
            aObjects.back().getObjectsNumber()
        }
    );
    return info;
}


int main() {

    {
        outHeader("1");

        vector<future<threadInfo>> results;

        for (int i = 0; i < 10; ++i) {
            results.emplace_back(async(launch::async, threadFunction, i));
        }

        cout << "Information about threads:" << endl;
        cout << "Name          Order     Number of A obects" << endl;

        for (auto& el : results) {
            threadInfo info = el.get();
            cout << info.name << "        " << info.number << "                " << info.numberOfObjects << endl;
        }
    }

    {
        outHeader("2");

        vector<int> data(10000);
        for (auto& el : data) {
            el = rand() % 100;
        }

        {
            Timer t{};
            long res = std::accumulate(cbegin(data), cend(data), 0);
            cout << "std::accumulate result: " << res << endl;
        }
        {
            Timer t{};
            long res = asyncAccumulate(cbegin(data), cend(data));
            cout << "asyncAccumulate auto result: " << res << endl;
        }
        {
            Timer t{};
            long res = asyncAccumulate(cbegin(data), cend(data), 1);
            cout << "asyncAccumulate 1 result: " << res << endl;
        }
        {
            Timer t{};
            long res = asyncAccumulate(cbegin(data), cend(data), 10);
            cout << "asyncAccumulate 10 result: " << res << endl;
        }
    }
/*
    {
        outHeader("3");

        mutex readLock;
        vector<future<int>> futures;
        const int dataSize = 4;
        futures.reserve(dataSize);

        for (int i = 0; i < 5; ++i) {
            futures.push_back(async(std::launch::async, [&readLock]() {
                string input;
                {
                    lock_guard<mutex> locker(readLock);
                    cin >> input;
                }
                return stoi(input);
            }));
        }

        for (auto& f : futures) {
            try {
                cerr << f.get() << endl;
            } catch (invalid_argument& e) {
                cout << e.what() << endl;
            } catch (out_of_range& e) {
                cout << e.what() << endl;
            }
        }
    }
*/
    {
        outHeader("4");

        ThreadSafeQueue<int> tsq;

        auto writer = [](ThreadSafeQueue<int>* p) {
            auto pause{1ms};
            for (int i = 0; i < 10; ++i) {
                pause = static_cast<std::chrono::milliseconds>(i);
                p->push(rand() % 100);
                this_thread::sleep_for(pause);
            }
        };

        auto reader = [](ThreadSafeQueue<int>* p) {
            auto pause{1ms};
            for (int i = 0; i < 15; ++i) {
                cout << *p->pop() << ' ';
            }
        };

        thread writerThread1(writer, &tsq);
        thread writerThread2(writer, &tsq);
        thread readerThread(reader, &tsq);

        writerThread1.join();
        writerThread2.join();
        readerThread.join();

    }

    {
        outHeader("6");

        vector<TaskFindMaxInt*> tasks;
        for (int i = 0; i < 10; ++i) {
            tasks.push_back(new TaskFindMaxInt({i, i + 20}));
        }
        ThreadPool tp;
        for (const auto& el : tasks) {
            tp.addTask(el);
        }

        this_thread::sleep_for(50ms);
        tp.stopPool();
        this_thread::sleep_for(50ms);

        for (const auto& el : tasks) {
            cout << el->getResult() << endl;
        }

        for (const auto& el : tasks) {
            delete el;
        }
    }

    {
        outHeader("5");
        Singleton& instance = Singleton::getInstance();
        (void)instance;
    }

//     cout << thread::hardware_concurrency() << endl;
}
