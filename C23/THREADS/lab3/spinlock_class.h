#pragma once
#include <atomic>
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


class spinlock_mtx {
	std::atomic_flag aflag = ATOMIC_FLAG_INIT;
public:
	//spinlock_mtx():aflag(ATOMIC_FLAG_INIT){}

	void lock() {
		while (aflag.test_and_set(std::memory_order_acquire));
	}

	bool try_lock() {
		return !aflag.test_and_set(
			std::memory_order_acquire
		);
	}

	void unlock() {
		aflag.clear(std::memory_order_release);
	}
};

// Структура для отправки своего исключения
struct empty_stack : std::exception
{
	std::string message{ "Container is empty" };
	const char* what() const noexcept
	{
		return message.c_str();
	}

};

// Потокобезопасный класс-контейнер
template <typename T>
class Thread_Safe_Vect {
private:
	std::vector<T> thr_vec;
	mutable spinlock_mtx mtx;
public:
	Thread_Safe_Vect() {}

	explicit Thread_Safe_Vect(const Thread_Safe_Vect &tsv_copy) {
		std::lock_guard<spinlock_mtx> lg_mtx(tsv_copy.mtx);
		thr_vec = tsv_copy.thr_vec;
	}

	auto operator=(const Thread_Safe_Vect &tsv_copy) {
		std::lock_guard<spinlock_mtx> lg_own_mtx(mtx);
		std::lock_guard<spinlock_mtx> lg_other_mtx(tsv_copy.mtx);
		thr_vec = tsv_copy.thr_vec;
	}

	void push(T nvalue) {
		std::lock_guard<spinlock_mtx> lg_mtx(mtx);
		thr_vec.push_back(nvalue);
	}

	void pop_back(T &value) {
		std::lock_guard<spinlock_mtx> lg_mtx(mtx);
		if (thr_vec.empty()) throw empty_stack();
		value = thr_vec.back();
		thr_vec.pop_back();
	}

	std::shared_ptr<T> pop_back() {
		std::lock_guard<spinlock_mtx> lg_mtx(mtx);
		if (thr_vec.empty()) throw empty_stack();
		const std::shared_ptr<T> res = std::make_shared<T>(thr_vec.back());
		thr_vec.pop_back();
		return res;
	}

	bool empty() const {
		std::lock_guard<spinlock_mtx> lg_mtx(mtx);
		return thr_vec.empty();
	}
};

