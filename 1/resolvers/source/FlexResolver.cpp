#include "FlexResolver.hpp"
#include <sstream>
#include <algorithm>

bool FlexResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
    std::string name, amount_str, lexems;
    unsigned amount;
    tokens["name"] = name;
    tokens["amount"] = amount_str;
    return true;
}