#include <future>
#include <string>


#include "pool.h"
#include "async_thrs.h"
#include "ts_queue.h"
#include "singleton_ts.h"

void main()
{
	// TASK 1
	/*
	// Future results vector
	std::vector<std::future<thr_definition>> f_res_vec;

	// Асинхронный запуск потоков.
	for (unsigned int i = 0; i < 15; ++i) {
		f_res_vec.emplace_back( std::async(std::launch::async, t1_thread_func, i ));
	}
	
	std::cout << "Thread name     Launch order     Objects number\n";

	for (auto &e : f_res_vec) {
		thr_definition res = e.get();
		std::cout << res.thr_name.c_str() << "        " << res.launch_order << "                " << res.objects_number << '\n';
	}
	*/
	// TASK 2
	std::cout << '\n';
	std::cout << "TASK 2. Async threads\n";

	srand(time(NULL));
	std::vector<int> acc_vect(1000000);
	for (auto &e : acc_vect) {
		
		e = rand() % 100;
		//std::cout << e << ' ';
	}
	std::cout << '\n';

	// Время перед запуском потоков
	auto start = std::chrono::steady_clock::now();
	long acc_res = std::accumulate(acc_vect.begin(), acc_vect.end(), 0);
	auto end = std::chrono::steady_clock::now();

	std::cout << "One thread accumulation result: " << acc_res << '\n';
	std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
	
	start = std::chrono::steady_clock::now();
	long par_acc_result = accumulation_async_func(acc_vect.begin(), acc_vect.end(), 0, 0);
	end = std::chrono::steady_clock::now();

	std::cout << "Multi thread accumulation result: " << par_acc_result << '\n';
	std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	// TASK 3
	std::cout << '\n';
	std::cout << "TASK 3. Handling exeptions by Future \n";
	std::mutex t3_mtx;
	std::vector<std::future<int>> fut_vec;
	fut_vec.reserve(5);

	for (int i = 0; i < 5; ++i) {
		fut_vec.push_back(std::async([&t3_mtx]() {
			std::string user_value;
			int iu_val;
			std::unique_lock<std::mutex> ulk(t3_mtx);
			std::cin >> user_value;
			ulk.unlock();
			iu_val = std::stoi(user_value);
			
			return iu_val;
		}));
	}

	for (auto &f : fut_vec) {
		try {
			std::cout << f.get() << '\n';
		} catch (std::invalid_argument &e) { 
			std::cout << e.what() << '\n'; 
		} catch( std::out_of_range &e ) { 
			std::cout << e.what() << '\n'; 
		}
	}

	// TASK 4
	// Потокобезопасная очередь
	std::cout << '\n';
	std::cout << "TASK 4. Thread safe queue\n";

	using namespace std::chrono_literals;

	thread_safe_queue<int> tsq_example;

	auto wr_func = [](thread_safe_queue<int> *tsv_p ) {
		std::chrono::milliseconds ms(1);
		for (int j = 0; j < 10; ++j) {
			ms = static_cast<std::chrono::milliseconds>(j);
			tsv_p->push(rand() % 100);
			std::this_thread::sleep_for(ms);
		}
	};

	auto read_func = []( thread_safe_queue<int> *tsv_p ) {
		std::chrono::milliseconds ms(3);
		for (int j = 0; j < 20; ++j) {
			int val;
			tsv_p->wait_and_pop(val);
			std::cout << val << ' ';
		}
	};

	std::thread writer_thr_1(wr_func, &tsq_example);
	std::thread writer_thr_2(wr_func, &tsq_example);
	std::thread reader_thr(read_func, &tsq_example);
	
	thread_safe_queue<int> tsq_copy_1 (tsq_example);
	thread_safe_queue<int> tsq_copy_2 = tsq_example;

	writer_thr_1.join();
	writer_thr_2.join();
	reader_thr.join();

	// TASK 5
	std::cout << '\n';
	std::cout << "TASK 5. Thread safe Singleton\n";

	Uniq_Singleton& instance = Uniq_Singleton::get_instance();
	
	// TASK 6
	/*
	std::cout << '\n';
	std::cout << "TASK 6. Pool of threads\n";
	int *result_mas[16]; // Массив указателей на результат выполнения каждой задачи.
	thread_task *tasks_collection[16]; // Коллекция заданий для пула потоков.
	

	for (int i = 0; i < 16; ++i) {
		result_mas[i] = new int(0);
		tasks_collection[i] = new thread_task(10+i, result_mas[i]);
	}
	
	//thread_task tt(10, result_mas[0]); 
	//tp.add_task( std::ref(tt) );

	threads_pool tpool; // Пул потоков.
	for (int i = 0; i < 16; ++i) {
		tpool.add_task(std::ref(*tasks_collection[i]));
	}

	// Засыпаем, чтобы потоки успели выполнить задачи.
 	std::this_thread::sleep_for( std::chrono::milliseconds(5) );
	tpool.stop_pool();

	std::cout << '\n';
	for (int i = 0; i < 16; ++i) {
		std::cout << *result_mas[i] << '\n';
		delete result_mas[i];
		if (tasks_collection[i] != nullptr) delete tasks_collection[i];
	
	*/
}