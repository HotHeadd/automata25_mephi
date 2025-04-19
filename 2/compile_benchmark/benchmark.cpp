#include <iostream>
#include <chrono>
#include "myre.hpp"

int main(){
	std::string regex;
	for (int i=0; i<3000; ++i){
		regex += 'a';
		if (i%2 == 0){
			regex += "{2,3}";
		}
		if (i%3 == 0){
			regex += "|";
		}
	}
	double sum=0;
	for (int i=0; i<10; ++i){
		auto start = std::chrono::high_resolution_clock::now();
		myre::compile(regex);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		sum += duration.count();
	}
	std::cout << "Average time is " << sum/10 << "\n";
	// 167 ms average
	return 0;
}