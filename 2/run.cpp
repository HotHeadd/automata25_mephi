#include <iostream>
#include "myre.hpp"

using namespace myre;

int main(){
	RegexParser parser;
	std::list<std::shared_ptr<Token>> tokens = parser.tokenize("aa*b{1}");
	for (auto token : tokens){
		if (token->type == TokenType::CHAR){
			std::cout << token->value << "\n";
		}
		if (token->type == TokenType::RANGE){
			std::cout << "RANGE " << token->start << " " << token->finish << "\n";
		}
	}
	return 0;
}