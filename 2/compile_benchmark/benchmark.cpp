#include <iostream>
#include <chrono>
#include "myre.hpp"

int main(){
	std::string regex;
	for (int i=0; i<10000; ++i){
		regex += 'a';
		if (i%2 == 0){
			regex += "{2,3}";
		}
		if (i%3 == 0){
			regex += "|";
		}
	}
	double sum=0;
	myre::RegexParser parser;
	myre::DFABuilder builder;
	myre::Context context;
	auto node = parser.parse(regex, context);
	for (int i=0; i<10; ++i){
		auto start = std::chrono::high_resolution_clock::now();
		builder.buildDFA(node, context);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		sum += duration.count();
	}
	std::cout << "Average time is " << sum/10 << " ms\n";
	// 130 ms average
	return 0;
}