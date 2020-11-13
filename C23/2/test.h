#ifndef C23_LAB_2_H
#define C23_LAB_2_H

#include <mutex>
#include <queue>
#include <memory>
#include <numeric>
#include <initializer_list>

// Task #1

class A {
    public:
        A() {
            std::lock_guard<std::mutex> locker(objectsCounterMutex);
            ++numberOfObjects;
        }
        A(A&& other) {
            std::lock_guard<std::mutex> locker(other.objectsCounterMutex);
            // Здесь может быть перемещение локальных данных класса
        }
        A(const A&) = delete;
        A& operator=(const A&) = delete;
        int getObjectsNumber() { return numberOfObjects; }
        static thread_local int numberOfObjects;
    private:
        std::mutex objectsCounterMutex;
};

// Task #2

template<typename Iterator> auto asyncAccumulate(
    Iterator first, Iterator last, int threadsNumber = 0
) -> typename std::remove_const<typename std::remove_reference<decltype(*first)>::type>::type {

    using T = decltype(asyncAccumulate<Iterator>(first, last));

    const auto size = std::distance(first, last);
    auto initValue = T{};
    auto stripSize = size;

    if (threadsNumber == 0) {
        threadsNumber = std::thread::hardware_concurrency();
        stripSize = size / threadsNumber;
    } else if (threadsNumber > 1) {
        stripSize = size / threadsNumber;
    }
    if (stripSize <= 1) {
        threadsNumber = 1;
        stripSize = 1;
    }

    if (threadsNumber == 1) {
        return std::accumulate(first, last, initValue);
    }

    Iterator next = first;
    std::vector<std::future<T>> results;
    while (next != last) {
        if (std::distance(next, last) < stripSize) stripSize = std::distance(next, last);
        std::advance(next, stripSize);
        results.emplace_back(std::async(std::launch::async, std::accumulate<Iterator, T>, first, next, initValue));
        first = next;
    }
    T sum{};
    for (auto& el : results) {
        sum += el.get();
    }
    return sum;
}

// Task #4

template <typename T> class ThreadSafeQueue {

    public:
        ThreadSafeQueue() {}
        ThreadSafeQueue(const ThreadSafeQueue& other) {
            std::lock_guard<std::mutex> locker(other.mtx);
            data = other.data;
        }
        ThreadSafeQueue& operator=(const ThreadSafeQueue& other) {
            std::unique_lock<std::mutex> thisLocker(mtx, std::defer_lock);
            std::unique_lock<std::mutex> otherLocker(other.mtx, std::defer_lock);
            std::lock(thisLocker, otherLocker);
            data = other.data;
        }
        size_t size() const {
            std::lock_guard<std::mutex> locker(mtx);
            return data.size();
        }
        bool empty() const {
            std::lock_guard<std::mutex> locker(mtx);
            return data.empty();
        }
        void push(const T& value) {
            std::lock_guard<std::mutex> locker(mtx);
            data.push(value);
            cv.notify_one();
        }
        void push(T&& value) {
            std::lock_guard<std::mutex> locker(mtx);
            data.push(std::move(value));
            cv.notify_one();
        }
        std::shared_ptr<T> pop() {
            std::unique_lock<std::mutex> locker(mtx);
            cv.wait(locker, [this] { return not data.empty(); });
            auto result = std::make_shared<T>(std::move(data.front()));
            data.pop();
            return result;
        }
        std::shared_ptr<T> tryPop() {
            std::lock_guard<std::mutex> locker(mtx);
            if (data.empty()) {
                return std::shared_ptr<T>{};
            }
            auto result = std::make_shared<T>(std::move(data.front()));
            data.pop();
            return result;
        }
    private:
        mutable std::mutex mtx;
        std::queue<T> data;
        std::condition_variable cv;
};

// Task #5

class Singleton {
    public:
        ~Singleton() {};
        static Singleton& getInstance();
    private:
        static std::unique_ptr<Singleton> instance;
        static std::once_flag flag;
        Singleton() {}
        Singleton(Singleton const&) = delete;
        Singleton& operator=(Singleton const&) = delete;
};

Singleton& Singleton::getInstance() {
    std::call_once(
        flag,
        [] { instance.reset(new Singleton); }
    );
    return *instance.get();
}

std::unique_ptr<Singleton> Singleton::instance;
std::once_flag Singleton::flag;

// Task #6

class Task {
    public:
        virtual ~Task() {}
        virtual void operator()() = 0;
};

class TaskFindMaxInt : public Task {
    public:
        TaskFindMaxInt(std::initializer_list<int> il) : data(il) {}
        void operator()() {
            result = 0;
            for (auto& el : data) result += el;
        }
        int getResult() { return result; }
    private:
        std::vector<int> data;
        int result = 10;
};

class ThreadPool {
    public:
        ThreadPool() {
            for (int i = 0; i < MaxThreads; ++i) {
                threads.emplace_back(&ThreadPool::taskThread, this);
            }
        }
        ~ThreadPool() {
            stopPool();
            for (auto& th : threads) {
                if (th.joinable()) th.join();
            }
        }
        void addTask(Task* task) {
            tasks.emplace(task);
        }
        void stopPool() { done = true; }
    private:
        void taskThread() {
            while (not done) {
                if (mtx.try_lock()) {
                    if (not tasks.empty()) {
                        auto task = tasks.front();
                        tasks.pop();
                        mtx.unlock();
                        (*task)();
                    } else {
                        mtx.unlock();
                    }
                }
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
        std::queue<Task*> tasks;
        static const int MaxThreads;
        std::vector<std::thread> threads;
        std::atomic_bool done{false};
        std::mutex mtx;
};

#endif // C23_LAB_2_H
