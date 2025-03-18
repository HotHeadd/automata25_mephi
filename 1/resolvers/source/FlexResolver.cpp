#include "FlexResolver.hpp"
#include <sstream>

bool FlexResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
	std::istringstream input(expression);
    lexer.switch_streams(&input, nullptr);
    return lexer.yylex();
}