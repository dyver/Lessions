#pragma once
#include <iostream>

class A {
public:
	void operator()() { std::cout << "In operator class A\n"; };
};