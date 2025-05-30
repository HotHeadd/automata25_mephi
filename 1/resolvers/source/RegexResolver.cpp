#include "RegexResolver.hpp"
#include <algorithm>

#define NAME "([a-zA-Z][a-zA-Z\\d]{0,15})"
#define INIT_LIST "(-?\\d+(,-?\\d+)*)?"

bool RegexResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
    static const std::regex arr_pattern{
        NAME  "\\["  "(\\d{0,9})"  "\\]"  "=\\{" INIT_LIST "\\}", 
        std::regex_constants::optimize}; // optimise конвертирует НКА в ДКА https://en.cppreference.com/w/cpp/regex/syntax_option_type
    std::smatch matches;
    bool result = std::regex_match(expression, matches, arr_pattern);
    if (result){
        const std::string& arr_size = matches[2].str(); // 2 группа захвата - кол-во элементов
        if ((arr_size.size() == 0 or arr_size == "0") and matches[3].str().size() == 0){
            result = false;
        }
        if (arr_size.size() and arr_size != "0" and
            std::stoi(arr_size) < std::count(expression.begin(), expression.end(),  ',')+1){
            result = false;
        }
    }
    if (result){
        tokens["name"] =  matches[1].str();
        tokens["amount"] = matches[2].str();
    }
    return result;
}
