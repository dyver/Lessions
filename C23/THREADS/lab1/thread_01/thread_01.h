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
#include <iomanip>
#include <mutex>
#include <exception>
#include <memory>

// Функция создания нового файла из измененной версии первого.
void file_edit(const char* filepath, char* newfile)
{
	std::ifstream ifile(filepath);
	if (!ifile) {
		std::cout << "CANNOT OPEN FILE " << filepath << '\n';
	}

	std::ofstream ofile(newfile);
	if (!ofile) {
		std::cout << "CANNOT OPEN FILE " << newfile << '\n';
	}

	// Итератором проходит по всем символам одного файла, применяет увеличение и записывает в новый файл.
	std::transform(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>(), 
		std::ostreambuf_iterator<char>(ofile), [](char ch) ->char{ return std::toupper(ch); }
	);
	
	ifile.close();
	ofile.close();
}

// Функция обработки контейнера несколькими потоками.
template<typename T_1, typename T_2>
void trans_func(const T_1 &t1, T_2 &t2)
{
	std::vector<std::thread> thr_vec; // Вектор потоков.
	
	int user_thr = std::thread::hardware_concurrency() - 1;
	std::cout << "Cores number: " << user_thr+1 << '\n';
	
	// Разделение контейнера для обработки потоками
	int delta = t1.size() / (user_thr + 1);
	std::cout << "Length of a container part for one thread : " << delta << '\n';
	int remainder = t1.size() % (user_thr + 1);
	std::cout << "Remainder : " << remainder << '\n';

	// Итераторы для перемещения по контейнерам
	auto src_it = t1.begin();
	auto res_it = t2.begin();

	// Лямбда для обработки части контейнера в потоке
	auto thr_func = [] (decltype(src_it) iter1, decltype(res_it) iter2, int num) {
			for (int i = 0; i < num; i++) {
			*iter2 = abs(*iter1);
			++iter1;
			++iter2;
		}
	};
	
	// Время перед запуском потоков
	auto start = std::chrono::steady_clock::now();

	// Добавление и запуск потоков
	for (int i = 0; i < user_thr; ++i) {
		thr_vec.emplace_back(thr_func, src_it, res_it, delta);
		std::advance(src_it, delta);
		std::advance(res_it, delta);
	}
	
	// Обработка последней части контейнера в основном потоке
	for (int i = 0; i < (delta + remainder); i++) {
		*res_it = abs(*src_it);
		++src_it;
		++res_it;
	}

	// Ожидание завершения дочерних потоков
	for (auto &elem : thr_vec) {
		if(elem.joinable()) elem.join();
	}
	
	// Результирующее время работы
	auto end = std::chrono::steady_clock::now();
	std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}

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
	mutable std::mutex mtx;
public:
	Thread_Safe_Vect(){}

	explicit Thread_Safe_Vect(const Thread_Safe_Vect &tsv_copy) {
		std::lock_guard<std::mutex> lg_mtx(tsv_copy.mtx);
		thr_vec = tsv_copy.thr_vec;
	}

	auto operator=(const Thread_Safe_Vect &tsv_copy) {
		std::lock_guard<std::mutex> lg_own_mtx(mtx);
		std::lock_guard<std::mutex> lg_other_mtx(tsv_copy.mtx);
		thr_vec = tsv_copy.thr_vec;
	}

	void push(T nvalue) {
		std::lock_guard<std::mutex> lg_mtx(mtx);
		thr_vec.push_back(nvalue);
	}

	void pop_back(T &value) {
		std::lock_guard<std::mutex> lg_mtx(mtx);
		if ( thr_vec.empty() ) throw empty_stack();
		value = thr_vec.back();
		thr_vec.pop_back();
	}

	std::shared_ptr<T> pop_back() {
		std::lock_guard<std::mutex> lg_mtx(mtx);
		if ( thr_vec.empty() ) throw empty_stack();
		const std::shared_ptr<T> res = std::make_shared<T>( thr_vec.back() );
		thr_vec.pop_back();
		return res;
	}

	bool empty() const {
		std::lock_guard<std::mutex> lg_mtx(mtx);
		return thr_vec.empty();
	}
};