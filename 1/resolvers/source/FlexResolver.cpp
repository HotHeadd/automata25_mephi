#include "FlexResolver.hpp"
#include <sstream>
#include <algorithm>

bool FlexResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
	std::istringstream input(expression);
    lexer.switch_streams(&input, nullptr);
    std::string name, amount_str, lexems;
    unsigned amount;
    if (lexer.yylex()){
        name = lexer.YYText();
        name.pop_back();
    }
    else{
        return false;
    }
    if (lexer.yylex()){
        amount_str = lexer.YYText();
        if (amount_str != ""){
            amount = std::stoi(amount_str);
        }
        else{
            amount = 0;
        }
    }
    else{
        return false;
    }
    if(lexer.yylex()){
        lexems = lexer.YYText();
        if (amount == 0 and lexems == "]={}"){
            return false;
        }
        if (amount != 0 and amount < std::count(lexems.begin(), lexems.end(), ',') + 1){
            return false;
        }
    }
    else{
        return false;
    }
    tokens["name"] = name;
    tokens["amount"] = amount_str;
    return true;
}