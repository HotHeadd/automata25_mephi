#include "RegexResolver.hpp"
#include <algorithm>
#include <fstream>

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


void RegexResolver::process_file(const std::string& in_filename, const std::string& out_filename){
    static const std::regex name_amount_pattern("([a-z]\\w{0,15})\\[(\\d*)\\]");
    std::string line;
    std::ifstream in(in_filename);
    // if (!in.is_open()){
    //     throw std::runtime_error("Wrong input filename format");
    // }
    std::ofstream out(out_filename);
    // if (!out.is_open()){
    //     throw std::runtime_error("Wrong output filename format");
    // }
    unsigned count=0;
    while(std::getline(in, line)){
        std::cout << line << "\n";
        if (is_suitable(line)){
            auto iter = std::sregex_iterator(line.begin(), line.end(), name_amount_pattern);
            std::smatch match = *iter;
            std::string name = match[1];
            std::string amount = match[2];
            out << count << ") " << name << " - " << amount << "\n";
        }
        ++count;
    }
}

} // namespace resolvers
