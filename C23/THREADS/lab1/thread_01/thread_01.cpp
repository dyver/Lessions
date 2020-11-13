#include "thread_01.h"

const char* oldfiles[] = { "1.txt", "2.txt", "3.txt", "4.txt" };
char* newfiles[] = { "n1.txt", "n2.txt", "n3.txt", "n4.txt" };

int main()
{
	// ***** TASK 1 *****
	std::cout << "TASK 1a\n";

	auto start = std::chrono::steady_clock::now();
	for (int i = 0;i < 4;i++) {
		std::thread thr(file_edit, oldfiles[i], newfiles[i]);
		if (thr.joinable()) thr.join(); // Все потоки запускаются последовательно.
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	std::cout << '\n';

	// ***** TASK 1b *****
	std::cout << "TASK 1b\n";

	std::vector<std::thread> f_threads; // НЕ инициализируется.

	start = std::chrono::steady_clock::now();
	for (int i = 0;i < 4;i++) {
		f_threads.emplace_back(file_edit, oldfiles[i], newfiles[i]);
	} // Потоки запускаются одновременно.

	for (unsigned int i = 0; i < f_threads.size(); i++) {
		if ( f_threads[i].joinable() ) f_threads[i].join();
	}
	end = std::chrono::steady_clock::now();
	std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	std::cout << '\n';
	
	// ***** TASK 1с *****
	std::cout << "TASK 1c\n";
	const int cores_num = std::thread::hardware_concurrency();
	//std::cout << "Cores number: " << cores_num << '\n';

	if (cores_num < 4) {
		// Запустить столько потоков, сколько ядер в системе.
	}
	else {
		std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
	}

	// ***** TASK 2 *****
	// Blocking threads for determined period of time.
	/*
	std::cout << "TASK 2. Blocking threads for determined period of time\n";

	std::chrono::milliseconds ms(2); // Задание интервала.
	
	std::cout << '\a';
	_beep(261, 300);
	_beep(293, 300);
	_beep(329, 300);
	_beep(349, 300);
	_beep(392, 300);
	_beep(440, 300);
	_beep(493, 300);
	*/
	std::cout << '\n';

	// ***** TASK 3 *****
	std::cout << "TASK 3\n";

	//int user_thr = cores_num-1;
	//std::cout << "Cores number: " << cores_num << '\n';
		
	std::vector<int> source{};
	
	// Заполнение источника
	for (int i = 1; i < 98; ++i	) {
		source.push_back(i*(-1));
	}

	// Объявление приемника и резервирование места чтобы можно было записывать не только в самый конец.
	std::vector<int> result(source.size());

	trans_func(source, result);
	
	for (auto &elem : result) {
		std::cout << elem << ' ';
	}
	std::cout << "\n\n";
	
	// ***** TASK 4 *****
	std::cout << "TASK 4\n";

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

	return 0;
}