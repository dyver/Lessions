#pragma once
#include <mutex>
#include <future>
#include <numeric>

// TASK 1

struct thr_definition {
	std::string thr_name;
	int launch_order;
	int objects_number;
};

class A {
	std::mutex amutex;
	static thread_local int counter;
public:
	A() { 
		std::lock_guard<std::mutex> lk_mtx(amutex);
		++counter; 
	}
	A(A&&) {}
	A(const A&) = delete;
	A& operator=(const A&) = delete;
	void set_counter() { std::lock_guard<std::mutex> lk_mtx(amutex);  counter = 1; }
	int get_counter() { return counter; }
};

thread_local int A::counter(0);

thr_definition t1_thread_func(int num)
{
	srand(time(NULL));
	// generate random number between 1 and 10
	unsigned int max_objects = rand() % 10 + 1;
	std::vector<A> ltvec;

	ltvec.emplace_back(A());
	ltvec.back().set_counter();
	for (unsigned int i = 0; i < max_objects-1; ++i) {
		ltvec.emplace_back( A() );
	}
	
	thr_definition th_def;
	th_def.thr_name = "thread_" + std::to_string(num);
	th_def.launch_order = num;
	th_def.objects_number = ltvec.back().get_counter();

	return th_def;
}

// TASK 2
template<typename Iterator, typename T>
T accumulation_async_func(Iterator first_iter, Iterator last_iter, T init, unsigned int cores_num)
{
	// Количество элементов в диаппазоне
	unsigned long const length = std::distance(first_iter, last_iter);
	unsigned long max_branch;
	unsigned int cores_number;

	if (cores_num == 0) {
		cores_number = std::thread::hardware_concurrency();
		max_branch = length / cores_number;
		if (max_branch < 0) max_branch = 1;
	}
	else if (cores_num == 1) {
		return std::accumulate(first_iter, last_iter, init);
	} 
	else {
		cores_number = cores_num;
		max_branch = length / cores_num;
		if (max_branch < 0) max_branch = 1;
	}

	if ( length <= max_branch ) {
		return std::accumulate(first_iter, last_iter, init);
	}
	else {
		Iterator next_iter = first_iter;
		std::advance(next_iter, max_branch);

		std::future<T> first_part_result =
			std::async(std::launch::async, accumulation_async_func<Iterator, T>, first_iter, next_iter, init, cores_number-1);
		
		T other_part_result = accumulation_async_func<Iterator, T>( next_iter, last_iter, T(), 1 );

		return first_part_result.get() + other_part_result;
	}
}