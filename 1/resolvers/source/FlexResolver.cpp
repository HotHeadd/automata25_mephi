#include "FlexResolver.hpp"
#include "generated.hpp"
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <iostream>

bool FlexResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
    std::string name, amount_str, lexems;
    bool res = lex(expression.c_str(), name, amount_str);
    tokens["name"] = name;
    tokens["amount"] = amount_str;
    return res;
}