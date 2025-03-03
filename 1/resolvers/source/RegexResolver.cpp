#include "RegexResolver.hpp"
#include <iostream>
#include <algorithm>

namespace resolvers
{

bool RegexResolver::is_suitable(const std::string& str){
    static const std::regex arr_pattern{
        "[a-z]\\w{0,15}\\[(0?\\]=\\{-?[1-9]\\d*(,-?[1-9]\\d*)*\\}|((\\d+)\\]=\\{(-?[1-9]\\d*(,-?[1-9]\\d*)*)?\\}*))", 
        std::regex_constants::icase};
    std::smatch matches;
    bool result = std::regex_match(str, matches, arr_pattern);
    if (result){
        const std::string& arr_size = matches[4].str(); // 4 группа захвата - кол-во элементов
        if (arr_size.size() and std::stoi(arr_size) < std::count(str.begin(), str.end(),  ',')+1){
            result = false;
        }
    }
    return result;
}


void RegexResolver::process_file(std::string_view in_file, std::string_view out_file){

}

} // namespace resolvers
