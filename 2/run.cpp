#include <iostream>
#include "myre.hpp"

using namespace myre;

int main(){
	RegexParser parser;
	std::list<std::shared_ptr<Token>> tokens = parser.tokenize("(a|b)*.c+");
	for (auto token : tokens){
		if (token->type == TokenType::CHAR){
			std::cout << token->value << "\n";
		}
		if (token->type == TokenType::RANGE){
			std::cout << "RANGE " << token->start << " " << token->finish << "\n";
		}
		if (token->type == TokenType::CONCAT){
			std::cout << "CONCAT\n";
		}
		if (token->type == TokenType::OR){
			std::cout << "OR\n";
		}
		if (token->type == TokenType::LPAR){
			std::cout << "(\n";
		}
		if (token->type == TokenType::RPAR){
			std::cout << ")\n";
		}
	}
	parser.parse("(a|b)*.c+");
	return 0;
}