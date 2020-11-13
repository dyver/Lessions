#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <ratio>
#include <vector>
#include <queue>
#include <iomanip>
#include <mutex>
#include <atomic>
#include <memory>

class thread_task // Класс функтор
{
	std::vector<int> task_vec;
	int* sum;

public:
	thread_task(int num, int* r) : task_vec(num) {
		sum = r;
		//std::cout << "vector size : " << task_vec.size() << '\n';
		for (int i = 0; i < task_vec.size(); ++i) {
			task_vec[i] = (rand() % 10);
		}
	}

	void operator()()
	{
		for (int i = 0; i < task_vec.size(); ++i) {
			*sum = *sum + task_vec[i];
		}
	}
};

// Класс обертка для вектора потоков. Гарантирует корректное завершение потоков.
class threads_wrapper {
	std::vector<std::thread> &threads;
public:
	explicit threads_wrapper(std::vector<std::thread> &new_thrs) : threads(new_thrs){}
	~threads_wrapper() {
		for (unsigned long i = 0; i < threads.size(); ++i) {
			if (threads[i].joinable()) threads[i].join();
		}
	}
};

// Пул потоков
class threads_pool {
	std::mutex mtx;
	std::atomic_bool done;
	std::queue<thread_task> tasks_qu;
	std::vector<std::thread> thr_vec;
	std::vector<std::thread::id> tvid;
	threads_wrapper twrap;

	int cores_number;

	void thread_func() {
		while (!done) { // Ждем условия завершения.
			if (mtx.try_lock() == true) {
				if (!tasks_qu.empty()) {
					thread_task tsk = tasks_qu.front();
					tasks_qu.pop();
					mtx.unlock();
					tsk();
				}
				else {
					mtx.unlock();
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

public:
	threads_pool() : done(false), twrap(thr_vec)
	{
		cores_number = std::thread::hardware_concurrency();
		std::cout << "Pool initialized with number of cores: " << cores_number << '\n';
		try {
			for (int i = 0; i < cores_number; ++i) {
				thr_vec.emplace_back(&threads_pool::thread_func, this);
				tvid.push_back(thr_vec.back().get_id());
				std::cout << "Thread " << tvid.back() << " initiated\n";
			}
		}
		catch (...) {
			done = true;
			throw;
		}
	}

	~threads_pool() {
		done = true;
	}

	void add_task(thread_task &tt) {
		std::lock_guard<std::mutex> mtg(mtx);
		tasks_qu.push(tt);
	}
	
	void stop_pool() { done = true;  }
};
