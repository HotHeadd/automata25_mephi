#include <iostream>
#include "myre.hpp"

int main(){
	std::string regex_str = "a*bc";
	std::string test = "aaaabc";
	myre::Regex regex(regex_str);
	bool res = myre::search(test, regex);
	if (res){
		std::cout << "String \"" << test << "\" matched regex \"" << regex_str << "\"\n";
	}
	else{
		std::cout << "ALL BAD GUYS\n";
	}
	return 0;
}