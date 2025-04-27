#include <iostream>
#include <chrono>
#include "myre.hpp"

using namespace myre;

int main(){
	std::string regex;
	for (int i = 0; i < 500; ++i) {
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

	int repeats = 15;
	for (int i=0; i<repeats; ++i){
		auto start = std::chrono::high_resolution_clock::now();
		builder.buildDFA(node, context);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		sum += duration.count();
	}
	std::cout << "Average compile time time is " << sum/repeats << " ms" << std::endl;
	// 75 ms average (sum time = average * repeats > 1 second)


	// runtime speed test
	std::string tmpl = "qwertyuiopasdfghjklzxcvbnm";
	std::string new_regex = "(" + tmpl + ")" + "*";
	std::string expr;
	for (int i=0; i<200000; ++i){
		expr += tmpl;
	}

	myre::DFA dfa = myre::compile(new_regex);
	int new_repeats = 20;
	int new_sum = 0;
	for (int i=0; i<new_repeats; ++i){
		auto start = std::chrono::high_resolution_clock::now();
		myre::fullmatch(expr, new_regex);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		new_sum += duration.count();
	}
	std::cout << "Average fullmatch time is: " << new_sum/new_repeats << " ms\n";
	return 0;
}