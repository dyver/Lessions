#include "chrono_test.h"
#include "obj_funcs.h"
#include "thr_classes.h"

void main()
{
	time_test_func();

	const int cores_num = std::thread::hardware_concurrency();
	std::cout << "Cores number: " << cores_num << '\n';

	auto start_time = std::chrono::high_resolution_clock::now();
	std::thread thr(thread_func);
	thr.join();
	auto end_time = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms\n";

	// Перемещение строки в поток
	std::string test_str("Hello thread");
	std::thread thr_str(thread_string_func, std::move(test_str));
	if (thr_str.joinable()) thr_str.join();
	try {
		std::cout << test_str.c_str() << '\n';
	}
	catch (...) {
		std::cout << "ERROR !\n";
	}

	// Объект-функция
	//std::thread obj_thr_1( A() ); // Не сработает
	std::thread obj_thr_2((A())); // Надо так
	std::thread obj_thr_3{ A() }; // Или так

	//obj_thr_1.join();
	obj_thr_2.join();
	obj_thr_3.join();

	// обертка в std::ref
	B bobj(1);
	std::cout << "In B class before thread work: " << bobj.get_param() << '\n';
	std::thread thr_B(thread_func_B, std::ref(bobj)); // Если не обернуть в ref, передастся копия даже по ссылке
	thr_B.join();
	std::cout << "In B class after thread work: " << bobj.get_param() << '\n';

	// template
	std::vector<int> ivec{4, 6, 8};
	std::thread vec_thr(thread_temp_func<decltype(ivec)>, std::ref(ivec));
	vec_thr.join();

	// emplace_back
	std::cout << '\n';
	std::vector<std::thread> threades_vec;
	for (char ch = 'A'; ch < 'Z'; ++ch) {
		threades_vec.emplace_back(thread_draw, ch, 10);
	}
	for (auto &elem : threades_vec) {
		elem.join();
	}
	std::cout << '\n';

	// lambda 
	int lnum = 0;
	std::thread lthr([&lnum]() {lnum++; }); // Передача по ссылке.
	lthr.join();
	std::cout << "\nlnum : " << lnum << '\n';

	std::vector<int> ivec2{ 4, 6, 8 };
	std::thread lthr2([&ivec2](){
		for (auto &elem : ivec2) {
			elem++;
		}
	});

	lthr2.join();
	for (auto &elem : ivec2) {
		std::cout << elem << ' ';
	}
	std::cout << '\n';

	// Вызов в потоке метода класса с параметром
	C c(3);
	std::thread thr_member_f( &C::inc, &c, 5 );
	thr_member_f.join();
	std::cout << "C class : " << c.get() << '\n';


}