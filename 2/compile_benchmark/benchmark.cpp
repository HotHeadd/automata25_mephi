#include <iostream>
#include <chrono>
#include "myre.hpp"

int main(){
	std::string regex;
	for (int i = 0; i < 700; ++i) {
		char c1 = 'a'+i%26;
		char c2 = 'a'+(i+1)%26;
		regex += "(";
		regex += c1;
		regex += "|";
		regex += c2;
		regex += ")";
		if (i % 3 == 0) {
			regex += "*";
		} else if (i % 5 == 0) {
			regex += "+";
		} else if (i % 7 == 0) {
			regex += "?";
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