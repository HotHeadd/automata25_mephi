#include <iostream>
#include <chrono>
#include "myre.hpp"

int main(){
	std::string regex = 
	"aaa*aaaa{2,3}aaaa|aaa{2,3}a|a*aa*aa{2,3}aa|a{2,3}aaa|"
	"bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa{2,3}a|a*aa*"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"{2,3}a|a*aa*aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbb"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"{2,3}a|a*aa*aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbb"
	"{2,3}a|a*aa*aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbb"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa"
	"aa{2,3}aa|a{2,3}aaa|bb{2,3}bbb{2,3}b{2,3}bbbaaa*aaaa{2,3}aaaa|aaa";
	double sum=0;
	for (int i=0; i<10; ++i){
		auto start = std::chrono::high_resolution_clock::now();
		myre::compile(regex);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		sum += duration.count();
	}
	std::cout << "Average time is " << sum/10 << "\n";
	// 10 ms average
	return 0;
}