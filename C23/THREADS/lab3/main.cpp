#include "spinlock_class.h"
#include "lock_free_stack.h"

void main()
{
	// TASK 1
	// Потокобезопасная очередь со своим Спинлоком
	std::cout << '\n';
	std::cout << "TASK 1. Spinlock stack\n";

	using namespace std::chrono_literals;

	// Потокобезопасный контейнер
	using namespace std::chrono_literals;

	Thread_Safe_Vect<int> tsv_example;

	auto wr_func = [](Thread_Safe_Vect<int> *tsv_p) {
		std::chrono::milliseconds ms(1);
		for (int j = 1; j < 10; ++j) {
			ms = static_cast<std::chrono::milliseconds>(j);
			tsv_p->push(j);
			std::this_thread::sleep_for(ms);
		}
	};

	auto read_func = [](Thread_Safe_Vect<int> *tsv_p) {
		std::chrono::milliseconds ms(3);
		for (int j = 1; j < 15; ++j) {
			try {
				int val;
				tsv_p->pop_back(val);
				std::cout << val;
			}
			catch (empty_stack &e) {
				std::cout << "_";
				std::this_thread::sleep_for(1ms);
			}
			//std::this_thread::sleep_for(1ms);
		}
	};

	std::thread writer_thr_1(wr_func, &tsv_example);
	std::thread reader_thr(read_func, &tsv_example);
	std::thread writer_thr_2(wr_func, &tsv_example);


	writer_thr_1.join();
	writer_thr_2.join();
	reader_thr.join();

	return;
}