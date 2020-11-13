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

std::mutex mtx;

void thread_func()
{
	mtx.lock();
	std::cout << "Thread id : " << std::this_thread::get_id() << '\n';
	std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));

	mtx.unlock();

}

void time_test_func()
{
	auto system_time = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(system_time); // Сутки назад system_time - std::chrono::hours(24)
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
	std::string s = std::ctime(&now_c); // Перевод времени в строку


	auto steady_time = std::chrono::high_resolution_clock::now();
	//std::time_t now_steady_t = std::chrono::system_clock::to_time_t(steady_time);

	steady_time = std::chrono::steady_clock::now(); // "Стабильные" часы. Не допускают коррекции времени системой - перевода стрелок.

	const int n = 1000;
	std::chrono::duration<long, std::ratio<1, n>> msn(2);

	std::cout << "Duration variable : " << msn.count() << "ms\n";
}

void thread_string_func(std::string s)
{
	std::cout << "In thread : " << s.c_str() << '\n';
}