#pragma once
class B {
	int param;
public:
	B(int par):param(par){}
	void inc() { param++; }
	int get_param(){ return param; }
};

void thread_func_B(B &bo) { bo.inc(); }

class C {
	int param;
public:
	C(int p = 0) { param = p; }
	int get() const { return param; }
	void inc(int b) { param += b; }
};

template<typename T_cont>
void thread_temp_func(T_cont &tt)
{
	std::cout << "\nIn template thread function\n";
	for (auto &e : tt) {
		e = e + 3;
		std::cout << e << ' ';
	}
}

void thread_draw(char ch, int num)
{
	for (int i = 0; i < num; ++i) {
		std::cout << ch;
	}
}