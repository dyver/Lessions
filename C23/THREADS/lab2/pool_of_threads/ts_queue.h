#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <queue>
#include <vector>
#include <iomanip>
#include <mutex>
#include <exception>
#include <memory>

template <typename T>
class thread_safe_queue
{
private:
	mutable std::mutex mtx;
	std::queue<T> data_queue;
	std::condition_variable cv;

public:
	thread_safe_queue() {}
	
	thread_safe_queue(const thread_safe_queue& tsq_source) {
			std::lock_guard<std::mutex> lk(tsq_source.mtx);
			data_queue = tsq_source.data_queue;
	}
	
	thread_safe_queue& operator=(const thread_safe_queue& tsq_source) {
		std::unique_lock<std::mutex> lg_own_mtx(mtx, std::defer_lock);
		std::unique_lock<std::mutex> lg_other_mtx(tsq_source.mtx, std::defer_lock);
		std::lock(lg_own_mtx, lg_other_mtx);
		data_queue = tsq_source.data_queue;
	}

	void push(T new_value) {
		std::lock_guard<std::mutex> lk(mtx);
		data_queue.push(std::move(new_value));
		cv.notify_one();
	}

	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> uk(mtx); // Не lock_guard, т.к. wait освобождает до момента появления новых данных, а потом снова захватит.
		cv.wait(uk, [this] { return !data_queue.empty(); });
		value = std::move_if_noexcept( data_queue.front() );
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop() {
		std::unique_lock<std::mutex> uk(mtx); // Не lock_guard, т.к. wait освобождает до момента появления новых данных, а потом снова захватит.
		cv.wait(uk, [this] { return !data_queue.empty(); });
		std::shared_ptr<T> result(
			std::make_shared<T> ( std::move(data_queue.front()) )
		);
		data_queue.pop();
		return result;
	}

	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lk(mtx);
		if (data_queue.empty()) return false;
		value = std::move_if_noexcept(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop() {
		std::lock_guard<std::mutex> lk(mtx);
		if (data_queue.empty()) {
			return std::shared_ptr<T>();
		}
		std::shared_ptr<T> result(
			std::make_shared<T>(std::move(data_queue.front()))
		);
		data_queue.pop();
		return result;
	}

	bool empty() const {
		std::lock_guard<std::mutex> lk(mtx);
		return data_queue.empty();
	}
};