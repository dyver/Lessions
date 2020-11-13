#pragma once
#include <mutex>
#include <memory>

class Uniq_Singleton {
public:
	~Uniq_Singleton() {};

	static Uniq_Singleton& get_instance();
private:
	static std::unique_ptr<Uniq_Singleton> sin_res_uptr;
	static std::once_flag sin_res_flag;

	Uniq_Singleton(void) {};
	
	Uniq_Singleton(Uniq_Singleton const&) = delete;
	Uniq_Singleton& operator= (Uniq_Singleton const&) = delete;
};

std::unique_ptr<Uniq_Singleton> Uniq_Singleton::sin_res_uptr;
std::once_flag Uniq_Singleton::sin_res_flag;

Uniq_Singleton& Uniq_Singleton::get_instance() {
	std::call_once(sin_res_flag, [] {
		sin_res_uptr.reset(new Uniq_Singleton);
		std::cout << "Initialization complited\n";
	});

	return *sin_res_uptr.get();
}
/*
class TS_Singleton {
private:
	std::unique_ptr<TS_Singleton> sin_res_uptr;
	std::once_flag sin_res_flag;

	//TS_Singleton() {}
	//~TS_Singleton() {}
	TS_Singleton(TS_Singleton const& ) = delete;
	TS_Singleton& operator= (TS_Singleton const&) = delete;
public:
	
	void init_tss() {
		std::call_once(sin_res_flag, &TS_Singleton::init_s, this);
	}
	static TS_Singleton& get_instance() {
		static TS_Singleton instance;
		return instance;
	}
};*/